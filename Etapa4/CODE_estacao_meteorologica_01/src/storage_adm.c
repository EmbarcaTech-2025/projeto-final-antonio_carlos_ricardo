#include <stdio.h>
#include "pico/stdlib.h"
#include "../include/storage.h"
#include "../include/storage_adm.h"
#include "../include/util.h"


static bool     aq_exist = false;
static uint64_t aq_cycle_min;
static uint64_t aq_cycle_max;
static uint32_t aq_index_min;
static uint32_t aq_index_max;

static bool     sent_exist = false;
static uint64_t sent_cycle_min;     // Só util no init, depois sem função
static uint64_t sent_cycle_max;
static uint32_t sent_index_min;     // Só util no init, depois sem função
static uint32_t sent_index_max;

static int32_t  sent_next_index;

/**
 * @brief Scaneia a Flash para mapear os indices das aquisições válidas de das já transmitidas
 * 
 * @return uint64_t, retorna o próximo cycle que deve ser usado 
 */
uint64_t storage_adm_init(){
    uint64_t buf_3[3];

    // Obtem os extremos da memoria utilizada
    printf("--- Flash State ---\n");
    for(int index=0; index<STORAGE_ADDRESS_DATA_N_ITEMS;index++){
        stored_data_read_3_uint64(index, buf_3);
        if(buf_3[0] != 0xFFFFFFFFFFFFFFFF){ // Há dado
            if(aq_exist){
                if(aq_cycle_min > buf_3[0]){aq_cycle_min = buf_3[0]; aq_index_min = index; }
                if(aq_cycle_max < buf_3[0]){aq_cycle_max = buf_3[0]; aq_index_max = index; }
            }else{
                aq_exist     = true;
                aq_cycle_min = buf_3[0];
                aq_cycle_max = buf_3[0];
                aq_index_min = index;
                aq_index_max = index;
            }

            if(buf_3[2] != 0xFFFFFFFFFFFFFFFF){ // Há foi enviado
                if(sent_exist){
                    if(sent_cycle_min > buf_3[0]){sent_cycle_min = buf_3[0]; sent_index_min = index; }
                    if(sent_cycle_max < buf_3[0]){sent_cycle_max = buf_3[0]; sent_index_max = index; }
                }else{
                    sent_exist     = true;
                    sent_cycle_min = buf_3[0];
                    sent_cycle_max = buf_3[0];
                    sent_index_min = index;
                    sent_index_max = index;
                }
            }    
        }
    }

    if(aq_exist){
        printf("Aq Exist\n");
        stored_data_read_3_uint64(aq_index_max, buf_3);
        printf("AqMax:   Index=%4d, Cycle=%5llu,   Cycle=%5llu, TAq=", aq_index_max, aq_cycle_max, buf_3[0]);
        util_print_timestamp(buf_3[1]);
        printf(", TSent="); 
        util_print_timestamp(buf_3[2]);
        printf("\n"); 

        stored_data_read_3_uint64(aq_index_min, buf_3);
        printf("AqMin:   Index=%4d, Cycle=%5llu,   Cycle=%5llu, TAq=", aq_index_min, aq_cycle_min, buf_3[0]);
        util_print_timestamp(buf_3[1]);
        printf(", TSent="); 
        util_print_timestamp(buf_3[2]);
        printf("\n"); 
    }else{
        printf("No Aq\n");
    }
    
    if(sent_exist){
        printf("Sent Exist\n");

        stored_data_read_3_uint64(sent_index_max, buf_3);
        printf("SentMax: Index=%4d, Cycle=%5llu,   Cycle=%5llu, TAq=", sent_index_max, sent_cycle_max, buf_3[0]);
        util_print_timestamp(buf_3[1]);
        printf(", TSent="); 
        util_print_timestamp(buf_3[2]);
        printf("\n"); 

        stored_data_read_3_uint64(sent_index_min, buf_3);
        printf("SentMin: Index=%4d, Cycle=%5llu,   Cycle=%5llu, TAq=", sent_index_min, sent_cycle_min, buf_3[0]);
        util_print_timestamp(buf_3[1]);
        printf(", TSent="); 
        util_print_timestamp(buf_3[2]);
        printf("\n"); 
    }else{
        printf("No Sent\n");
    }

    sent_next_index = -1;

    if(aq_exist) return aq_cycle_max + 1;
            else return 1;
}

/**
 * @brief define o indice de onde a próxima aquisição deverá ser salva
 * 
 * @return int32_t 
 */
int32_t  storage_adm_get_next_aq_index(){
    uint32_t old_aq_index_max;
    uint32_t old_aq_index_min;
    // Variaveis utilizadas:
    // static bool     aq_exist = false;
    // static uint32_t aq_index_min;
    // static uint32_t aq_index_max;
    // static bool     sent_exist = false;
    // static uint32_t sent_index_max;

    if(!aq_exist){
        // ainda não há dados na flash
        aq_exist       = true;
        aq_index_min   = 0;
        aq_index_max   = 0;
        sent_exist     = false;
        sent_index_max = 0; // inrelevante pois ainda não existe
        return aq_index_max;
    }

    old_aq_index_max = aq_index_max;

    aq_index_max++;
    if(aq_index_max >= STORAGE_ADDRESS_DATA_N_ITEMS){
        aq_index_max = 0;
    }

    // detecta se aq_index_max entrou num novo bloco
    bool new_block = !(aq_index_max & 0x000F);  // final 0 implica novo bloco

    old_aq_index_min = aq_index_min;
    if(new_block){
        // verificar se o está no mesmo bloco, se estiver é necessário mudar
        if((aq_index_max & 0xFFFFFFF0) == (aq_index_min & 0xFFFFFFF0)){
            aq_index_min = (aq_index_min & 0xFFFFFFF0) + 0x00000010;

            bool flip_min = false;
            if(aq_index_min >= STORAGE_ADDRESS_DATA_N_ITEMS){
                aq_index_min = 0;
                flip_min     = true;
            }

            // se mudou o mínimo pode ser necessário atualizar o sent_index_max, só se ele existir
            if(sent_exist){
                if(old_aq_index_min == sent_index_max){
                    sent_exist = false; // como eles eram iguais ao aumentar o aq_index_min o sent para de existir
                }else if(old_aq_index_min < sent_index_max){
                    if(flip_min){
                        sent_exist = false;   // certamente passou o sent, pois o sent está acima e o aq fez flip
                    } else {    // precisa ver o novo 
                        if(aq_index_min > sent_index_max){
                            // o novo aq_min superou o sent, portanto está inválido
                            sent_exist = false;
                        }
                    }  
                }else{  // old_aq_index_min > sent_index_max
                    // no máximo se igualam num flip
                }    
            }

            // se mudou o mínimo pode ser necessário atualizar o sent_index_max, só se ele existir
            if(sent_next_index >= 0){
                if(old_aq_index_min == sent_next_index){
                    sent_next_index = -1; // como eles eram iguais ao aumentar o aq_index_min o sent para de existir
                }else if(old_aq_index_min < sent_next_index){
                    if(flip_min){
                        sent_next_index = -1;   // certamente passou o sent, pois o sent está acima e o aq fez flip
                    } else {    // precisa ver o novo 
                        if(aq_index_min > sent_next_index){
                            // o novo aq_min superou o sent, portanto está inválido
                            sent_next_index = -1;
                        }
                    }  
                }else{  // old_aq_index_min > sent_next_index
                    // no máximo se igualam num flip
                }    
            }
        }
    }
    return aq_index_max;
}

/**
 * @brief Retorna o indice da aquisição que deve ser enviada para o servidor
 * 
 * @return int32_t, retorna < 0 caso não haja aquisição que deva ser enviada 
 */
int32_t  storage_adm_get_next_send_index(){
    sent_next_index = -1;
    if(sent_exist){
        if(sent_index_max != aq_index_max){
            sent_next_index = sent_index_max + 1;
            if(sent_next_index >= STORAGE_ADDRESS_DATA_N_ITEMS){
                sent_next_index = 0;
            }    
        }
    }else{
        if(aq_exist){
            sent_next_index = aq_index_min;
        }else{
            // Não há o que enviar, pois não há dados
        }
    }

    return sent_next_index;
}

/**
 * @brief Avisa que a ultima aquisição informada por: storage_adm_get_next_aq_index(); foi transmitida com sucesso
 * @return true, faça o update da aquisição 
 * @return false, não faça o update, pois este endereço da memoria foi utilizado por outra aquisição 
 */
bool storage_adm_was_sent(){
    if(sent_next_index >= 0){
        sent_index_max = sent_next_index;
        sent_exist = true;
        return true;
    }else return false;
}
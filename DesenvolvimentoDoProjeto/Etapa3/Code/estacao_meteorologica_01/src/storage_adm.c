#include <stdio.h>
#include "pico/stdlib.h"
#include "../include/storage.h"
#include "../include/storage_adm.h"

static bool ativa_print = false; //true;

static int32_t index_data_min;  // -1 não há dados, iguais quando só há um dado
static int32_t index_data_max;

static bool    index_last_send_valid;    // se é true index_last_send tem o indice do último envidao
static int32_t index_last_send;          // se é false o último enviado é anterior a index_data_min

static int32_t index_no_sent_min;  // -1 se tiver que enviar,tem que(index_no_sent_max >= 0) e o valor será index_data_min
static int32_t index_no_sent_max;  // -1 não há dados para enviar
static int32_t index_last_sending; // -1 não pode ser atualizado o sending


static int32_t index_sent_min;
static int32_t index_sent_max;


void storage_adm_print(char * msg){
    if(!ativa_print) return;
    printf(msg);
    printf("Index data(%d,%d) no sent(%d,%d) last send:%d\n",
        index_data_min, index_data_max,
        index_no_sent_min, index_no_sent_max,
        index_last_sending
    );
}

uint64_t storage_adm_init(){
    bool first_data    = true;  
    bool first_sent    = true;
    bool first_no_sent = true;

    index_data_min     = -1;
    index_data_max     = -1;

    index_no_sent_min  = -1;
    index_no_sent_max  = -1;
    index_last_sending = -1;

    index_sent_min     = -1;
    index_sent_max     = -1;

    uint64_t data_min,    data_max;
    uint64_t sent_min,    sent_max;
    uint64_t no_sent_min, no_sent_max;

    data_max = 0xFFFFFFFFFFFFFFFF;
    uint64_t *data_pointer = (uint64_t *)(XIP_BASE + STORAGE_OFFSET_DATA_FIRST);

    for(int32_t i = 0; i< STORAGE_ADDRESS_DATA_N_ITEMS;i++){
        if((*data_pointer) != 0xFFFFFFFFFFFFFFFF){
            if(first_data){
                first_data     = false;
                index_data_min = i;
                index_data_max = i;
                data_min = *data_pointer;
                data_max = *data_pointer;
            }else{
                if(data_min > *data_pointer){
                    data_min       = *data_pointer;
                    index_data_min = i;
                } 
                if(data_max < *data_pointer){
                    data_max       = *data_pointer;
                    index_data_max = i;
                }
            }

            if(data_pointer[2] == 0xFFFFFFFFFFFFFFFF){
                // not send
                if(first_no_sent){
                    first_no_sent     = false;
                    index_no_sent_min = i;
                    index_no_sent_max = i;
                    no_sent_min = *data_pointer;
                    no_sent_max = *data_pointer;
                }else{
                    if(no_sent_min > *data_pointer) {
                        no_sent_min       = *data_pointer;
                        index_no_sent_min = i;
                    }
                    if(no_sent_max < *data_pointer) {
                        no_sent_max = *data_pointer;
                        index_no_sent_max = i;
                    }
                }
            }else{
                // sent
                if(first_sent){
                    first_sent     = false;
                    index_sent_min = i;
                    index_sent_max = i;
                    sent_min = *data_pointer;
                    sent_max = *data_pointer;
                }else{
                    if(sent_min > *data_pointer) {
                        sent_min = *data_pointer;
                        index_sent_min = i;
                    }
                    if(sent_max < *data_pointer) {
                        sent_max = *data_pointer;
                        index_sent_max = i;
                    }
                }
            }
        }
        data_pointer += 32;
    }

    if(first_sent){
        index_last_send_valid = false;
    }else{
        index_last_send_valid = true;
        index_last_send       = index_sent_max;
    }

    storage_adm_print("Init ");

//    printf("storage_adm_init data_max = %d\n", data_max);
    return data_max;
}

static void data_min_plus_one(){
    if(index_last_sending >=0){
        if(index_last_sending == index_data_min){
            index_last_sending = -1;
        }
    }

    if(index_last_send_valid){
        if(index_last_send == index_data_min){
            index_last_send_valid = false;
        }
    }

    if(index_no_sent_min >=0){
        if(index_no_sent_min == index_data_min){
            index_no_sent_min = -1;
        }
    }

    index_data_min++;
    if(index_data_min >= STORAGE_ADDRESS_DATA_N_ITEMS){
        index_data_min = 0;
    }
}


int32_t storage_adm_get_next_index(){
    storage_adm_print("Next Start ");
    if(index_data_max < 0){
        index_data_max = 0;
        index_data_min = 0;
    }else if(index_data_min == index_data_max){
        index_data_max++;
        if(index_data_max >= STORAGE_ADDRESS_DATA_N_ITEMS){
            index_data_max = 0;
        }
    }else if(index_data_min < index_data_max){
        index_data_max++;
        if(index_data_max >= STORAGE_ADDRESS_DATA_N_ITEMS){
            index_data_max = 0;
            if(index_data_min == 0){
                data_min_plus_one();
            }
        }
    }else{  //index_data_min > index_data_max
        index_data_max++;   // como o min é maior não passará do maximo
        if(index_data_min == index_data_max){
            data_min_plus_one();
        }
    }
    index_no_sent_max = index_data_max;

    storage_adm_print("Next end ");
    return index_data_max;
}


int32_t storage_adm_get_next_sent(){
    // Obs.: o index_no_sent_min só deveria avançar com a confirmação do envio
    storage_adm_print("NextSend Start ");

    if(index_last_send_valid){
        if(index_last_send == index_data_max){
            index_last_sending = -1;
        }else{
            index_last_sending = index_last_send + 1;
            if(index_last_sending >= STORAGE_ADDRESS_DATA_N_ITEMS){
                index_last_sending = 0;
            }
        }
    }else{
        index_last_sending = index_data_min;
    }

    
    
/*


    if(index_no_sent_max < 0){
        index_last_sending = -1;
    }else{
        if(index_no_sent_min < 0){
            index_no_sent_min  = index_data_min;
        }else{
            index_no_sent_min++;
            if(index_no_sent_min >= STORAGE_ADDRESS_DATA_N_ITEMS){
                index_no_sent_min = 0;
            }    
        }
        index_last_sending = index_no_sent_min;
        if(index_no_sent_min == index_no_sent_max){
            index_no_sent_min = -1;
            index_no_sent_max = -1;
        }
    }

    storage_adm_print("NextSend end ");*/
    return index_last_sending;
}

int32_t storage_adm_can_update_lest_send(){
    if(index_last_sending >= 0){
        index_last_send_valid = true;
        index_last_send       = index_last_sending;
        return index_last_sending;
    }else{
        return -1;
    }
}
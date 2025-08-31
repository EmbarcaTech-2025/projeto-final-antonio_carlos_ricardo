#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "hardware/flash.h"
#include "../include/config_data.h"
#include "../include/storage.h"

void __not_in_flash_func(stored_conf_clear)() {
//void stored_conf_clear(){
    uint32_t ints = save_and_disable_interrupts();
    flash_range_erase(STORAGE_OFFSET_CONF, STORAGE_CLEAR_BLOCK_SIZE);
    restore_interrupts(ints);
}
void stored_conf_read(){
    memcpy(&config_data, (uint8_t *)(XIP_BASE + STORAGE_OFFSET_CONF), sizeof(config_data));
}
void __not_in_flash_func(stored_conf_write)(){
//void stored_conf_write(){
    uint32_t ints = save_and_disable_interrupts();
    //stored_conf_clear();
    flash_range_erase(STORAGE_OFFSET_CONF, STORAGE_CLEAR_BLOCK_SIZE);
    flash_range_program(STORAGE_OFFSET_CONF, (uint8_t *)&config_data, sizeof(config_data));
    restore_interrupts(ints);
}

// ---------- Função de escrita na flash (em RAM) ----------
void  __not_in_flash_func(stored_data_clear)(int32_t index){
    uint32_t ints = save_and_disable_interrupts();
    flash_range_erase(STORAGE_OFFSET_DATA_FIRST + ((index >> 4) * STORAGE_CLEAR_BLOCK_SIZE), STORAGE_CLEAR_BLOCK_SIZE);
    restore_interrupts(ints);
}



void    stored_data_clear_all(){
    printf("stored_data_clear_all\n");
    sleep_ms(200);
     for(int i=0;i<STORAGE_ADDRESS_DATA_N_ITEMS;i+=16){
        stored_data_clear(i);
        printf("Limpando i=%d\n", i);
        //sleep_ms(200);
    }
}
void    stored_data_read( int32_t index, EstationData * estation_data){
    memcpy(estation_data, (const uint8_t *)(XIP_BASE + STORAGE_OFFSET_DATA_FIRST + (index * STORAGE_ADDRESS_DATA_SIZE)), sizeof(EstationData));
}
void __not_in_flash_func(stored_data_write)(int32_t index, EstationData * estation_data, bool update){
    if((!(index & 0x000F))&&(!update)) stored_data_clear(index);

    uint32_t ints = save_and_disable_interrupts();
    flash_range_program(STORAGE_OFFSET_DATA_FIRST + (index * STORAGE_ADDRESS_DATA_SIZE), (uint8_t *) estation_data, sizeof(EstationData));
    restore_interrupts(ints);
}


bool stored_data_get_last_write_index(uint64_t * max_cycle,  int32_t  * index){
    uint64_t *data_pointer = (uint64_t *)(XIP_BASE + STORAGE_OFFSET_DATA_FIRST);

    bool ha_dado = false;

    uint64_t t_max_cycle =  0;
    int32_t  t_index     = -1;

    for(int i=0;i<STORAGE_ADDRESS_DATA_N_ITEMS;i++){
        if(*data_pointer != 0xFFFFFFFFFFFFFFFF){
            if(ha_dado){
                if(t_max_cycle < (*data_pointer)){
                    t_max_cycle = (*data_pointer);
                    t_index     = i;
                }
            }else{
                ha_dado = true;
                t_max_cycle = (*data_pointer);
                t_index     = i;
            }
        }
    }    
    if(ha_dado){
        *max_cycle = t_max_cycle;
        *index     = t_index;
    }else{
        *max_cycle =  0;
        *index     = -1;
    }
    return ha_dado;
}
int32_t stored_data_get_last_send_index(int32_t last_write_index){
    int32_t  index  = -1;

    uint64_t *data_pointer = (uint64_t *)(XIP_BASE + STORAGE_OFFSET_DATA_FIRST + (last_write_index * STORAGE_ADDRESS_DATA_SIZE));
    uint64_t *data_pointer2;
    data_pointer2 = data_pointer-3;
    for(int i = last_write_index; (i >=0) && (index < 0); i--){
        if(*data_pointer != 0xFFFFFFFFFFFFFFFF){
            if(*(data_pointer + 2) != 0xFFFFFFFFFFFFFFFF){
                index = i;
            }    
        }    
        data_pointer -= 64;
    }

    data_pointer = (uint64_t *)(XIP_BASE + STORAGE_OFFSET_DATA_FIRST + ((STORAGE_ADDRESS_DATA_N_ITEMS - 1) * STORAGE_ADDRESS_DATA_SIZE));
    for(int i = (STORAGE_ADDRESS_DATA_N_ITEMS - 1); (i >last_write_index) && (index < 0); i--){
        if(*data_pointer != 0xFFFFFFFFFFFFFFFF){
            if(*(data_pointer + 2) != 0xFFFFFFFFFFFFFFFF){
                index = i;
            }    
        }    
        data_pointer -= 64;
    }

    return index;
}


void stored_printf(bool include_conf){
    uint32_t address = include_conf? 0:16;
    for(;address < (STORAGE_ADDRESS_DATA_N_ITEMS + 0x0010);address++){
        uint8_t * p = (uint8_t *)(XIP_BASE + STORAGE_OFFSET_CONF + (address * STORAGE_ADDRESS_DATA_SIZE));

        printf("%4d, %4x,", address, address);
        for(int i=0;i<32;i++){
            for(int j=0;j<8;j++){
                printf("%02x ", *p);
                p += 1;
            }
            if(i<31) printf(",");
        }
        printf("\r\n");
    
    }
}
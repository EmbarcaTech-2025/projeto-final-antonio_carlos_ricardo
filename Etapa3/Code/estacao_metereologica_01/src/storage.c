#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/flash.h"
#include "../include/config_data.h"
#include "../include/storage.h"


void stored_conf_clear(){
    flash_range_erase(STORAGE_OFFSET_CONF, STORAGE_CLEAR_BLOCK_SIZE);
}
void stored_conf_read(){
    memcpy(&config_data, (uint8_t *)(XIP_BASE + STORAGE_OFFSET_CONF), sizeof(config_data));
}
void stored_conf_write(){
    stored_conf_clear();
    flash_range_program(STORAGE_OFFSET_CONF, (uint8_t *)&config_data, sizeof(config_data));
}


void    stored_data_clear(int32_t index){
    flash_range_erase(STORAGE_OFFSET_DATA_FIRST + ((index >> 4) * STORAGE_CLEAR_BLOCK_SIZE), STORAGE_CLEAR_BLOCK_SIZE);
}
void    stored_data_clear_all(){
    for(int i=0;i<STORAGE_ADDRESS_DATA_N_ITEMS;i+=16){
        stored_data_clear(i);
    }
}
void    stored_data_read( int32_t index, EstationData * estation_data){
    memcpy(estation_data, (const uint8_t *)(XIP_BASE + STORAGE_OFFSET_DATA_FIRST + (index * STORAGE_ADDRESS_DATA_SIZE)), sizeof(EstationData));
}
void stored_data_write(int32_t index, EstationData * estation_data, bool update){
    if((!(index & 0x000F))&&(!update)) stored_data_clear(index);
    flash_range_program(STORAGE_OFFSET_DATA_FIRST + (index * STORAGE_ADDRESS_DATA_SIZE), (uint8_t *) estation_data, sizeof(EstationData));
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
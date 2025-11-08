/**
 * @file    storage.c
 * @author  Carlos e Ricardo
 * @brief   Rotinas para leitura e escrita na Flash
 * Obs.: O outro core tem que estar rodando em RAM, e com suas interrupções paradas
 * Obs.: As rotinas aqui só administram o Core corrente
 * @version 0.1
 * @date    2025-09-16
 * 
 * @copyright Copyright (c) 2025
 */
#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "hardware/flash.h"
#include "../include/config_data.h"
#include "../include/storage.h"

/**
 * @brief Limpa a área da memória referente a configuração
 * 
 */
void __not_in_flash_func(stored_conf_clear)() {
//void stored_conf_clear(){
    uint32_t ints = save_and_disable_interrupts();
    flash_range_erase(STORAGE_OFFSET_CONF, STORAGE_CLEAR_BLOCK_SIZE);
    restore_interrupts(ints);
}

/**
 * @brief Lê a configuração que está na flash e coloca em config_data
 * 
 */
void stored_conf_read(){
    memcpy(&config_data, (uint8_t *)(XIP_BASE + STORAGE_OFFSET_CONF), sizeof(config_data));
}

/**
 * @brief Limpa a área da memória referente a configuração e grava nela os dados que estão em config_data
 * 
 */
void __not_in_flash_func(stored_conf_write)(){
//void stored_conf_write(){
    uint32_t ints = save_and_disable_interrupts();
    flash_range_erase(STORAGE_OFFSET_CONF, STORAGE_CLEAR_BLOCK_SIZE);
    flash_range_program(STORAGE_OFFSET_CONF, (uint8_t *)&config_data, sizeof(config_data));
    restore_interrupts(ints);
}

/**
 * @brief Limpa um bloco de aquisições
 * 
 */
// ---------- Função de escrita na flash (em RAM) ----------
void  __not_in_flash_func(stored_data_clear)(int32_t index){
    uint32_t ints = save_and_disable_interrupts();
    flash_range_erase(STORAGE_OFFSET_DATA_FIRST + ((index >> 4) * STORAGE_CLEAR_BLOCK_SIZE), STORAGE_CLEAR_BLOCK_SIZE);
    restore_interrupts(ints);
}

/**
 * @brief Limpa todos os blocos/dados de aquisições da Flash
 * 
 */
void    stored_data_clear_all(){
    printf("stored_data_clear_all\n");
    sleep_ms(200);
     for(int i=0;i<STORAGE_ADDRESS_DATA_N_ITEMS;i+=16){
        stored_data_clear(i);
        printf("Limpando i=%d\n", i);
    }
}

/**
 * @brief Lê uma aquisição especificada pelo seu index e coloca no buffer estation_data
 * 
 * @param index 
 * @param estation_data 
 */
void    stored_data_read( int32_t index, EstationData * estation_data){
    memcpy(estation_data, (const uint8_t *)(XIP_BASE + STORAGE_OFFSET_DATA_FIRST + (index * STORAGE_ADDRESS_DATA_SIZE)), sizeof(EstationData));
}
/**
 * @brief Lê os primeiros 3 uint64_t da aquisição(numero, data/hora da aquisição, data/hora envio servidor) definida pelo index
 * 
 * @param index 
 * @param buf_3_uint64_t 
 */
void    stored_data_read_3_uint64(int32_t index, uint64_t * buf_3_uint64_t){
    memcpy(buf_3_uint64_t, (const uint8_t *)(XIP_BASE + STORAGE_OFFSET_DATA_FIRST + (index * STORAGE_ADDRESS_DATA_SIZE)), 24);
}

/**
 * @brief Salva o dado na Flash, limpando o bloco se necessário
 * - ATENÇÃO:
 *   -As interrupções de ambos os cores devem estar desabilitadas
 *   -Os COREs só podem estar rodando na RAM
 * @param index 
 * @param estation_data 
 * @param update, quando true ==> não limpa a flash só sobre escreve
 */
void __not_in_flash_func(stored_data_write)(int32_t index, EstationData * estation_data, bool update){
    if((!(index & 0x000F))&&(!update)) {
        //stored_data_clear(index);
        flash_range_erase(STORAGE_OFFSET_DATA_FIRST + ((index >> 4) * STORAGE_CLEAR_BLOCK_SIZE), STORAGE_CLEAR_BLOCK_SIZE);
    }

    flash_range_program(STORAGE_OFFSET_DATA_FIRST + (index * STORAGE_ADDRESS_DATA_SIZE), (uint8_t *) estation_data, sizeof(EstationData));
}


/**
 * @brief Faz o DUMP da memória flash em Hexa, via printf
 * 
 * @param include_conf 
 */
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
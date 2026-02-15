/**
 * @file    storage.h
 * @author  Carlos e Ricardo
 * @brief   Rotinas para leitura e escrita na Flash
 * Obs.: O outro core tem que estar rodando em RAM, e com suas interrupções paradas
 * Obs.: As rotinas aqui só administram o Core corrente
 * @version 0.1
 * @date    2025-09-16
 * @version 0.2
 * @date    2026-01-14
 * 
 * @copyright Copyright (c) 2025
 */
#ifndef STORAGE_H
#define STORAGE_H

//#include "estation_data.h"

#define STORAGE_CLEAR_BLOCK_SIZE        0x1000      // 4k
#define STORAGE_WRITE_BLOCK_SIZE        0x0100      // 256

#define STORAGE_OFFSET_CONF             0x0100000
#define STORAGE_OFFSET_DATA_FIRST       0x0101000   // = STORAGE_ADDRESS_CONF + 0x1000
#define STORAGE_ADDRESS_DATA_SIZE       0x0100      // 256 bytes
#define STORAGE_ADDRESS_DATA_N_ITEMS    0xFF0       // 4080 = 255 blocos de 4k bytes
                                                    //      multiplos de 16 pois: 4k/256 = 16
                                                    //      STORAGE_CLEAR_BLOCK / STORAGE_ADDRESS_DATA_SIZE

/**
 * @brief Limpa a área da memória referente a configuração
 * 
 */
void stored_conf_clear();

/**
 * @brief Lê a configuração que está na flash e coloca em config_data
 * 
 */
void stored_conf_read(uint8_t *data, int size);

/**
 * @brief Limpa a área da memória referente a configuração e grava nela os dados que estão em config_data
 * 
 */
void stored_conf_write(uint8_t *data, int size);

/**
 * @brief Limpa um bloco de aquisições
 * 
 */
void    stored_data_clear(int32_t index);

/**
 * @brief Limpa todos os blocos/dados de aquisições da Flash
 * 
 */
void    stored_data_clear_all();



#endif // STORAGE_H
/**
 * @file    storage.h
 * @author  Carlos e Ricardo
 * @brief   Rotinas para leitura e escrita na Flash
 * Obs.: O outro core tem que estar rodando em RAM, e com suas interrupções paradas
 * Obs.: As rotinas aqui só administram o Core corrente
 * @version 0.1
 * @date    2025-09-16
 * 
 * @copyright Copyright (c) 2025
 */
#ifndef STORAGE_H
#define STORAGE_H

#include "estation_data.h"

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
void stored_conf_read();
/**
 * @brief Limpa a área da memória referente a configuração e grava nela os dados que estão em config_data
 * 
 */
void stored_conf_write();

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
/**
 * @brief Lê uma aquisição especificada pelo seu index e coloca no buffer estation_data
 * 
 * @param index 
 * @param estation_data 
 */
void    stored_data_read( int32_t index, EstationData * estation_data);
/**
 * @brief Lê os primeiros 3 uint64_t da aquisição(numero, data/hora da aquisição, data/hora envio servidor) definida pelo index
 * 
 * @param index 
 * @param buf_3_uint64_t 
 */
void    stored_data_read_3_uint64(int32_t index, uint64_t * buf_3_uint64_t);
/**
 * @brief Salva o dado na Flash, limpando o bloco se necessário
 * - ATENÇÃO:
 *   -As interrupções de ambos os cores devem estar desabilitadas
 *   -Os COREs só podem estar rodando na RAM
 * @param index 
 * @param estation_data 
 * @param update, quando true ==> não limpa a flash só sobre escreve
 */
void    stored_data_write(int32_t index, EstationData * estation_data, bool update);

/**
 * @brief Faz o DUMP da memória flash em Hexa, via printf
 * 
 * @param include_conf 
 */
void stored_printf(bool include_conf);


#endif // STORAGE_H
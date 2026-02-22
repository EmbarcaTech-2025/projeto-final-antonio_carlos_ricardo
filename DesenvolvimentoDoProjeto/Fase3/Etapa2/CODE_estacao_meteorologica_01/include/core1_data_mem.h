/**
 * @file    core1_data_mem.h
 * @author  Carlos e Ricardo
 * @brief   Faz operações de leitura e escrita dos dados da estação na memoria Flash
 * - bloqueia o acesso a flash durante as operações de escrita
 * 
 * @version 0.1
 * @date    2025-09-05
 * 
 * @copyright Copyright (c) 2025
 */
#ifndef CORE1_DATA_MEM_H
#define CORE1_DATA_MEM_H


/**
 * @brief Lê o dado da estação que está na Flash
 * 
 * @param index, define o local da memoria a onde o dado está
 * @param estation_data, ponteiro para onde copiar o dado
 */
void core1_data_mem_read(uint32_t index, EstationData * estation_data);

/**
 * @brief Salva o dado da estação na Flash
 * - se necessário limpa o bloco de memória antes
 * - bloqueia o acesso a flash durante as operações de escrita
 * 
 * @param index, define o local da memoria a onde o dado será salvo
 * @param estation_data, ponteiro do dado a ser salvo
 */
void core1_data_mem_save(uint32_t index, EstationData * estation_data);

/**
 * @brief Atualiza o dado da estação Flash
 * - Utilizado para salvar o t_send
 * - NÃO limpa a memória
 * - Antes de fazer o update verifica se o cycle continua válido
 * 
 * @param index, define o local da memoria a onde o dado será salvo
 * @param estation_data, dado a ser salvo
 */
void core1_data_mem_update(uint32_t index, EstationData * estation_data);


#endif // CORE1_DATA_MEM_H
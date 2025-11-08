/**
 * @file    core1_acquisition.h
 * @author  Carlos e Ricardo
 * @brief   Inicializa os sensores e faz as aquisições
 * @version 0.1
 * @date    2025-09-16
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef CORE1_ACQUISITION_H
#define CORE1_ACQUISITION_H

#include "estation_data.h"


/**
 * @brief Inicializa os sensores
 * 
 * @param next_cycle, número da próxima aquisição 
 */
void core1_acquisition_init(uint64_t next_cycle);
/**
 * @brief Executa a aquisição dos sensores e disponibiliza os resultados na variável: core1_estation_data
 * 
 */
void core1_acquisition_get();


#endif // CORE1_ACQUISITION_H
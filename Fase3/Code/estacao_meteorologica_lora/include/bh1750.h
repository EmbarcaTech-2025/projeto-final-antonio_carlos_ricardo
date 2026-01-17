/**
 * @file    bh1750.h
 * @author 
 * @brief   Inicializa e lê dados do sensor de luminosidade
 * @version 0.1
 * @date    2025-09-17
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef BH1750_H
#define BH1750_H

/**
 * @brief Inicializa o BH1750 em modo contínuo de medição de alta resolução.
 * 
 */
void bh1750_init();
/**
 * @brief Le a intensidade luminosa
 * 
 * @return uint16_t 
 */
uint16_t bh1750_read_lux();


#endif // BH1750_H
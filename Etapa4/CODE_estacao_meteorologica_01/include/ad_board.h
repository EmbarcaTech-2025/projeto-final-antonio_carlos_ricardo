/**
 * @file    ad_board.h
 * @author  Carlos e Ricardo
 * @brief   Rotinas de inicialização e uso do Ad interno da BitDogLab
 * @version 0.1
 * @date    2025-09-05
 * 
 * @copyright Copyright (c) 2025
 */
#ifndef AD_BOARD_H
#define AD_BOARD_H


/**
 * @brief Inicialização do Ad interno da BitDogLab
 */
void     ad_boad_init();
/**
 * @brief Leitura do Ad com conexão externa (default microfone)
 * 
 * @return uint16_t, valor do AD
 */
uint16_t ad_boad_get_value();
/**
 * @brief Leitura do Ad conectado ao eixo X do Joystick
 * 
 * @return uint16_t, valor do AD
 */
uint16_t ad_boad_get_joystick_x();
/**
 * @brief Leitura do Ad conectado ao eixo Y do Joystick
 * 
 * @return uint16_t, valor do AD
 */
uint16_t ad_boad_get_joystick_y();


#endif // AD_BOARD_H
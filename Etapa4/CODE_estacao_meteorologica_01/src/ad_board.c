/**
 * @file    ad_board.c
 * @author  Carlos e Ricardo
 * @brief   Rotinas de inicialização e uso do Ad interno da BitDogLab
 * @version 0.1
 * @date    2025-09-05
 * 
 * @copyright Copyright (c) 2025
 */
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "../include/ad_board.h"


/**
 * @brief Inicialização do Ad interno da BitDogLab
 */
void     ad_boad_init(){
    adc_init();
    adc_gpio_init(26);
    adc_gpio_init(27);
    adc_gpio_init(28);
}
/**
 * @brief Leitura do Ad com conexão externa (default microfone)
 * 
 * @return uint16_t, valor do AD
 */
uint16_t ad_boad_get_value(){
    adc_select_input(2);
    sleep_us(2);
    return adc_read();
}
/**
 * @brief Leitura do Ad conectado ao eixo X do Joystick
 * 
 * @return uint16_t, valor do AD
 */
uint16_t ad_boad_get_joystick_x(){
    adc_select_input(1);
    sleep_us(2);
    return adc_read();
}
/**
 * @brief Leitura do Ad conectado ao eixo Y do Joystick
 * 
 * @return uint16_t, valor do AD
 */
uint16_t ad_boad_get_joystick_y(){
    adc_select_input(0);
    sleep_us(2);
    return adc_read();
}
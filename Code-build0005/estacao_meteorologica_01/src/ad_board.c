#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "../include/ad_board.h"


void     ad_boad_init(){
    adc_init();
    adc_gpio_init(26);
    adc_gpio_init(27);
    adc_gpio_init(28);
}
uint16_t ad_boad_get_value(){
    adc_select_input(2);
    sleep_us(2);
    return adc_read();
}
uint16_t ad_boad_get_joystick_x(){
    adc_select_input(1);
    sleep_us(2);
    return adc_read();
}
uint16_t ad_boad_get_joystick_y(){
    adc_select_input(0);
    sleep_us(2);
    return adc_read();
}


/**
 * @file    testes.h
 * @author  Carlos e Ricardo
 * @brief   Rotinas para testes de acesso aos sensores I2C
 * @version 0.1
 * @date    2025-09-16
 * 
 * @copyright Copyright (c) 2025
 */
#include <pico/cyw43_arch.h>
#include "../include/testes.h"
#include "../lib/include/aht10.h"
#include "../lib/include/bmp280_main.h"
#include "../lib/include/bh1750.h"
#include "../lib/include/rtc_ds3231.h"


/**
 * @brief   Rotinas para testes de acesso aos sensores I2C
 * 
 */
void testes_i2c(){
    aht10_init();
    while(1){
        Aht10Data d = aht10_read();
        printf("Temp = %.2f    Humi==%.2f\n", d.temp, d.humidity);

        sleep_ms(100);
    }


    bh1750_init();
    while(1){
        int16_t l = bh1750_read_lux();
        printf("luminosidade = %lu lux\n", l);
    }


    bmp280_main_init();
    while(1){
        sensors_t s;
        s = bmp280_main_get_all_0();
        printf("Temp=%.2f  press=%d  alt=%.1f\n", s.temperature, s.pressure, s.altitude);
        sleep_ms(500);
    }

    rtc_ds3231_test();
}
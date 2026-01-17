#include <stdio.h>
#include "pico/stdlib.h"
#include "../include/aq_data_bme280.h"
#include "../include/code_config.h"
#include "../include/loop_printf.h"
#include "src/bmp280/bmp280.h"

int aqdatabme280_init_power_on(){
    return 0;
}
int aqdatabme280_init_aq(){
    return 0;
}
int aqdatabme280_read(AqDataBme280_Value *value){
    bmp280_main_init();
    sensors_t bmp280 = bmp280_main_get_all_0();
    if(DEBUG_ON_BMP280){
        loop_printf("BMP280-Altitude = %f meters\n", bmp280.altitude);
        loop_printf("BMP280-Temp     = %f \n",       bmp280.temperature);
        loop_printf("BMP280-press    = %d \n",       bmp280.pressure);
    }
    value->humidity = 0xFF;
    value->temp     = bmp280.temperature * 100;
    value->press    = 60000 - (bmp280.pressure / 2);
    return 0;
}
int aqdatabme280_sleep(){
    return 0;
}
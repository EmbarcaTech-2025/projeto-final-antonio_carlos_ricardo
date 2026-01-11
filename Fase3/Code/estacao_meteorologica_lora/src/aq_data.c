#include "hardware/i2c.h"
#include "../include/aq_data.h"
#include "../include/code_config.h"

int aqdata_init( AqData *value){
    // Inicializar I2Cs
    i2c_init(         I2C_MAIN_BUS,      I2C_MAIN_BAUDRATE);
    gpio_pull_up(     I2C_MAIN_GPIO_SDA);
    gpio_pull_up(     I2C_MAIN_GPIO_SCL);
    gpio_set_function(I2C_MAIN_GPIO_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_MAIN_GPIO_SCL, GPIO_FUNC_I2C);

    if(value == NULL) return 0xFFFF;
    int ret = 0;
    if(value->active_sensors.battery){
        if(aqdatabat_init()) ret |= AQ_ITEM_BAT_VALUE;
    }
    if(value->active_sensors.bme280){
        if(aqdatabme280_init()) ret |= AQ_ITEM_BME280;
    }
    if(value->active_sensors.gps){
        if(aqdatagps_init()) ret |= AQ_ITEM_GPS;
    }
    if(value->active_sensors.lux){
        if(aqdatalux_init()) ret |= AQ_ITEM_LUX;
    }
    return ret;
}
int aqdata_read( AqData *value){
    if(value == NULL) return 0xFFFF;
    int ret = 0;
    if(value->active_sensors.battery){
        if(aqdatabat_read(&value->battery)) ret |= AQ_ITEM_BAT_VALUE;
    }
    if(value->active_sensors.bme280){
        if(aqdatabme280_read(&value->bme280)) ret |= AQ_ITEM_BME280;
    }
    if(value->active_sensors.gps){
        if(aqdatagps_read(&value->gps)) ret |= AQ_ITEM_GPS;
    }
    if(value->active_sensors.lux){
        if(aqdatalux_read(&value->lux)) ret |= AQ_ITEM_LUX;
    }
    return ret;
}
int aqdata_sleep(AqData *value){
    if(value == NULL) return 0xFFFF;
    int ret = 0;
    if(value->active_sensors.battery){
        if(aqdatabat_sleep()) ret |= AQ_ITEM_BAT_VALUE;
    }
    if(value->active_sensors.bme280){
        if(aqdatabme280_sleep()) ret |= AQ_ITEM_BME280;
    }
    if(value->active_sensors.gps){
        if(aqdatagps_sleep()) ret |= AQ_ITEM_GPS;
    }
    if(value->active_sensors.lux){
        if(aqdatalux_sleep()) ret |= AQ_ITEM_LUX;
    }
    return ret;
}
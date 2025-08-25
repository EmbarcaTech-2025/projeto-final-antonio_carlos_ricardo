#include <string.h>
#include "pico/stdlib.h"
#include "../include/config_data.h"
#include "../include/storage.h"

#define CONFIG_DATA_SUN_START_VALUE 0x12345678
#define CONFIG_DATA_XOR_START_VALUE 0x87
#define CONFIG_DATA_VERSION         0x00000001

ConfigData config_data;



static uint8_t config_data_error_check(bool set){
    int       size = sizeof(config_data) - 8;
    uint8_t * p    = (uint8_t *)&config_data;

    uint32_t sum = CONFIG_DATA_SUN_START_VALUE;
    uint8_t  xor = CONFIG_DATA_XOR_START_VALUE;

    for(int i=0;i<size;i++){
        sum += *p;
        xor ^= *p;
        p++;
    }

    uint8_t ret = 0;
    if(config_data.version != CONFIG_DATA_VERSION) ret != 0x01;
    if(sum                 != config_data.sum)     ret |= 0x02;
    if(xor                 != config_data.xor)     ret |= 0x04;

    if(set){
        config_data.sum = sum;
        config_data.xor = xor;
    }

    return ret;
}


void config_data_set_default(){
    config_data.version = CONFIG_DATA_VERSION;

    strcpy(config_data.station_name,      "Station000");
    strcpy(config_data.station_token,     "12345");
    strcpy(config_data.station_group,     "Grupo000");
    strcpy(config_data.station_latitude,  "00.00");
    strcpy(config_data.station_longitude, "00.00");
    strcpy(config_data.station_elevation, "00.00");

    strcpy(config_data.wifi_ssid,         "MEU_WIFISSID");
    strcpy(config_data.wifi_psw,          "WIFI_PSW");
    strcpy(config_data.server_url,        "Ip_do_servidor");
    config_data.server_port = 8080;

    config_data.period_acquisition = PERIOD_ACQUISITION_MIN;
    config_data.period_send        = PERIOD_SEND_MIN;
    config_data.period_show        = PERIOD_2SEC; 
    config_data.period_sync_clk    = PERIOD_10MIN; 

    config_data.rtc_with                      = true;

    config_data.sensor_i2c_pressure           = true;
    config_data.sensor_i2c_humidity           = true;
    config_data.sensor_i2c_luminosity         = true;
    config_data.sensor_i2c_wind_direction     = false;

    config_data.sensor_digital_wind_speed     = false;
    config_data.sensor_digital_rain_intensity = false;

    config_data.sensor_analog_ar_co            = 0xFF;
    config_data.sensor_analog_ar_co2           = 0xFF;
    config_data.sensor_analog_ar_o3            = 0xFF;
    config_data.sensor_analog_soil_temperature = 0xFF;
    config_data.sensor_analog_soil_humidity    = 0xFF;
    config_data.sensor_analog_uv_index         = 0xFF;

    config_data_error_check(true);
}


uint32_t config_data_get_period_in_sec(Period period){
    switch(period){
        case PERIOD_1SEC:   return     1;
        case PERIOD_2SEC:   return     2;
        case PERIOD_5SEC:   return     5;
        case PERIOD_10SEC:  return    10;
        case PERIOD_20SEC:  return    20;
        case PERIOD_1MIN:   return    60;
        case PERIOD_2MIN:   return   120;
        case PERIOD_5MIN:   return   300;
        case PERIOD_10MIN:  return   600;
        case PERIOD_20MIN:  return  1200;
        case PERIOD_1HOUR:  return  3600;
        case PERIOD_2HOUR:  return  7200;
        case PERIOD_6HOUR:  return 21600;
        case PERIOD_12HOUR: return 43200;
        case PERIOD_1DAY:   return 86400;
    }
}



bool config_data_storage_read(){
    stored_conf_read();
    if(config_data_error_check(false)){
        config_data_set_default();
        return false;
    }else{
        return true;
    }
}

void config_data_storage_write(){
    config_data.version = CONFIG_DATA_VERSION;
    config_data_error_check(true);
    stored_conf_write();
}


char * config_data_period_string(Period period){
    switch(period){
        case PERIOD_1SEC:   return "1 s";
        case PERIOD_2SEC:   return "2 s";
        case PERIOD_5SEC:   return "5 s";
        case PERIOD_10SEC:  return "10 s";
        case PERIOD_20SEC:  return "20 s";
        case PERIOD_1MIN:   return "1 min";
        case PERIOD_2MIN:   return "2 min";
        case PERIOD_5MIN:   return "5 min";
        case PERIOD_10MIN:  return "10 min";
        case PERIOD_20MIN:  return "20 min";
        case PERIOD_1HOUR:  return "1 hora";
        case PERIOD_2HOUR:  return "2 hora";
        case PERIOD_6HOUR:  return "6 hora";
        case PERIOD_12HOUR: return "12 hora"; 
        case PERIOD_1DAY:   return "1 dia";
    }
}
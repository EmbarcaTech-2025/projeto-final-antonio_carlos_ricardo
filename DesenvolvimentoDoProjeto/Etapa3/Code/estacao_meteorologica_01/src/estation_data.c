#include <stdio.h>
#include "pico/stdlib.h"
#include "../include/config_data.h"
#include "../include/estation_data.h"
#include "../include/storage.h"

static uint64_t next_cycle;
static uint32_t next_free_index;
static uint32_t next_send_index;

char estation_data_buf_tmp[256+128];

void     estation_data_clear(EstationData * estation_data){
    uint8_t * p;
    p = ( uint8_t *)estation_data;

    for(int i=0;i<sizeof(EstationData);i++){
        p[i] = 0xFF;
    }
}


int16_t estation_data_json(EstationData * estation_data, char * buf){
    int16_t size = 0;

    size += sprintf(&(buf[size]), "Last Data:{\"station\":%s,\"token\":%s,\"group\":%s,\"cycle\":%llu,\"t_aquire\":%llu,\"sensors\":{", 
        config_data.station_name,
        config_data.station_token,
        config_data.station_group,
        estation_data->cycle,
        estation_data->t_aquire
        );

    if(config_data.sensor_i2c_pressure){
        size += sprintf(&(buf[size]), "\"press\":");
        if(estation_data->press == 0xFFFFFFFF)
             size += sprintf(&(buf[size]), ",");
        else size += sprintf(&(buf[size]), "%.2f,", estation_data->press * 0.01);

        size += sprintf(&(buf[size]), "\"tempp\":");
        if(estation_data->temp_p == 0xFFFFFFFF)
             size += sprintf(&(buf[size]), ",");
        else size += sprintf(&(buf[size]), "%.2f,", estation_data->temp_p * 0.01);
    }
    if(config_data.sensor_i2c_humidity){
        size += sprintf(&(buf[size]), "\"humi\":");
        if(estation_data->humidity == 0xFFFFFFFF)
             size += sprintf(&(buf[size]), ",");
        else size += sprintf(&(buf[size]), "%.2f,", estation_data->humidity * 0.01);

        size += sprintf(&(buf[size]), "\"temph\":");
        if(estation_data->temp_h == 0xFFFFFFFF)
             size += sprintf(&(buf[size]), ",");
        else size += sprintf(&(buf[size]), "%.2f,", estation_data->temp_h * 0.01);

    }
    if(config_data.sensor_i2c_luminosity){
        size += sprintf(&(buf[size]), "\"lumi\":");
        if(estation_data->luminos == 0xFFFFFFFF)
             size += sprintf(&(buf[size]), ",");
        else size += sprintf(&(buf[size]), "%u,", estation_data->luminos);
    }
    if(config_data.sensor_i2c_wind_direction){
        size += sprintf(&(buf[size]), "\"windd\":");
        if(estation_data->wind_dir == 0xFFFFFFFF)
             size += sprintf(&(buf[size]), ",");
        else size += sprintf(&(buf[size]), "%u,", estation_data->wind_dir);
    }


    if(config_data.sensor_digital_wind_speed){
        size += sprintf(&(buf[size]), "\"winds\":");
        if(estation_data->wind_speed == 0xFFFFFFFF)
             size += sprintf(&(buf[size]), ",");
        else size += sprintf(&(buf[size]), "%.2f,", estation_data->wind_speed * 0.01);
    }
    if(config_data.sensor_digital_rain_intensity){}

    if(config_data.sensor_analog_ar_co){}
    if(config_data.sensor_analog_ar_co2){}
    if(config_data.sensor_analog_ar_o3){}
    if(config_data.sensor_analog_soil_temperature){}
    if(config_data.sensor_analog_soil_humidity){}
    if(config_data.sensor_analog_uv_index){}



    


    size += sprintf(&(buf[size]), "\"X\":}}");
    return size;
}








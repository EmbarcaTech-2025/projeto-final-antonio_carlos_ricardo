#include <stdio.h>
#include <time.h>
#include "pico/stdlib.h"
#include "../include/config_data.h"
#include "../include/core_communication.h"
#include "../include/estation_data.h"
#include "../include/storage.h"

static uint64_t next_cycle;
static uint32_t next_free_index;
static uint32_t next_send_index;

char estation_data_buf_tmp[512];

void     estation_data_clear(EstationData * estation_data){
    uint8_t * p;
    p = ( uint8_t *)estation_data;

    for(int i=0;i<sizeof(EstationData);i++){
        p[i] = 0xFF;
    }
}


int16_t estation_data_json_full_fake(EstationData * estation_data, char * buf, uint64_t aq_n){
    int size = sprintf(buf, "{" 
        "\"name\":\"1234567890abcdef\","
        "\"token\":\"1234567890abcdef\","
        "\"group\":\"1234567890abcdef\"," 
        "\"lat\":-123.456789,"
        "\"lon\":-123.456789,"
        "\"elev\":10000.2,"
        "\"time_aq\":\"2025-08-29T12:53:13.298237953Z\","
        "\"aq_n\":%llu,"
        "\"sensors\":{"
            "\"press\":1013.25,"
            "\"temp_p\":123.45,"
            "\"humi\":100.00,"
            "\"temp_h\":123.45,"
            "\"lumi\":30000.12,"
//          "\"lumi_c": tbd, 
            "\"wind_d\":321.25,"
            "\"wind_s\":27.12,"
            "\"rain\":200.12,"
            "\"co\":100000.12,"
            "\"co2\":100000.12,"
            "\"o3\":100000.12,"
            "\"soil_t\":123.45,"
            "\"soil_h\":100.00,"
            "\"uv_index\":12.34"
            "}"
        "}", aq_n);
    return size;
}

int16_t estation_data_json(EstationData * estation_data, char * buf){
    int16_t size = 0;

    char buf_tmp[32];
    struct tm tm_utc;
    gmtime_r(&(estation_data->t_aquire), &tm_utc);
    strftime(buf_tmp, 31, "%Y-%m-%dT%H:%M:%SZ", &tm_utc);

    size += sprintf(&(buf[size]), "{\"name\":\"%s\",\"token\":\"%s\",\"group\":\"%s\",\"lat\":%s,\"lon\":%s,\"elev\":%s,\"aq_n\":%llu,\"time_aq\":\"%s\",\"sensors\":{", 
        config_data.station_name,
        config_data.station_token,
        config_data.station_group,
        config_data.station_latitude,
        config_data.station_longitude,
        config_data.station_elevation,
        estation_data->cycle,
        buf_tmp
        );


        
    bool have_sensor = false;
    
    if(config_data.sensor_i2c_pressure){
        if(have_sensor) size += sprintf(&(buf[size]), ",");
        have_sensor = true;

        size += sprintf(&(buf[size]), "\"press\":");
        if(estation_data->press != 0xFFFFFFFF) size += sprintf(&(buf[size]), "%.2f", estation_data->press * 0.01);
                                          else size += sprintf(&(buf[size]), "null");
        size += sprintf(&(buf[size]), ",");

        size += sprintf(&(buf[size]), "\"temp_p\":");
        if(estation_data->temp_p != 0xFFFFFFFF) size += sprintf(&(buf[size]), "%.2f", estation_data->temp_p * 0.01);
                                           else size += sprintf(&(buf[size]), "null");
    }
    if(config_data.sensor_i2c_humidity){
        if(have_sensor) size += sprintf(&(buf[size]), ",");
        have_sensor = true;
        
        size += sprintf(&(buf[size]), "\"humi\":");
        if(estation_data->humidity != 0xFFFFFFFF) size += sprintf(&(buf[size]), "%.2f", estation_data->humidity * 0.01);
                                             else size += sprintf(&(buf[size]), "null");
        size += sprintf(&(buf[size]), ",");

        size += sprintf(&(buf[size]), "\"temp_h\":");
        if(estation_data->temp_h != 0xFFFFFFFF) size += sprintf(&(buf[size]), "%.2f", estation_data->temp_h * 0.01);
                                           else size += sprintf(&(buf[size]), "null");
    }
    if(config_data.sensor_i2c_luminosity){
        if(have_sensor) size += sprintf(&(buf[size]), ",");
        have_sensor = true;

        size += sprintf(&(buf[size]), "\"lumi\":");
        if(estation_data->luminos != 0xFFFFFFFF) size += sprintf(&(buf[size]), "%u", estation_data->luminos);
                                            else size += sprintf(&(buf[size]), "null");
    }
    if(config_data.sensor_i2c_wind_direction){
        if(have_sensor) size += sprintf(&(buf[size]), ",");
        have_sensor = true;

        size += sprintf(&(buf[size]), "\"wind_d\":");
        if(estation_data->wind_dir != 0xFFFFFFFF) size += sprintf(&(buf[size]), "%u", estation_data->wind_dir * 0.01);
                                             else size += sprintf(&(buf[size]), "null");
    }
    if(config_data.sensor_digital_wind_speed){
        if(have_sensor) size += sprintf(&(buf[size]), ",");
        have_sensor = true;

        size += sprintf(&(buf[size]), "\"wind_s\":");
        if(estation_data->wind_speed != 0xFFFFFFFF) size += sprintf(&(buf[size]), "%.2f", estation_data->wind_speed * 0.01);
                                               else size += sprintf(&(buf[size]), "null");
    }
    if(config_data.sensor_digital_rain_intensity){
        if(have_sensor) size += sprintf(&(buf[size]), ",");
        have_sensor = true;

        size += sprintf(&(buf[size]), "\"rain\":");
        if(estation_data->rain_intensity != 0xFFFFFFFF) size += sprintf(&(buf[size]), "%.2f", estation_data->rain_intensity * 0.01);
                                                   else size += sprintf(&(buf[size]), "null");
    }
    if(config_data.sensor_analog_ar_co != 0xFF){
        if(have_sensor) size += sprintf(&(buf[size]), ",");
        have_sensor = true;

        size += sprintf(&(buf[size]), "\"co\":");
        if(estation_data->analog_ar_co != 0xFFFFFFFF) size += sprintf(&(buf[size]), "%.2f", estation_data->analog_ar_co * 0.01);
                                                 else size += sprintf(&(buf[size]), "null");
    }
    if(config_data.sensor_analog_ar_co2 != 0xFF){
        if(have_sensor) size += sprintf(&(buf[size]), ",");
        have_sensor = true;

        size += sprintf(&(buf[size]), "\"co2\":");
        if(estation_data->analog_ar_co2 != 0xFFFFFFFF) size += sprintf(&(buf[size]), "%.2f", estation_data->analog_ar_co2 * 0.01);
                                                  else size += sprintf(&(buf[size]), "null");
    }
    if(config_data.sensor_analog_ar_o3 != 0xFF){
        if(have_sensor) size += sprintf(&(buf[size]), ",");
        have_sensor = true;

        size += sprintf(&(buf[size]), "\"o3\":");
        if(estation_data->analog_ar_o3 != 0xFFFFFFFF) size += sprintf(&(buf[size]), "%.2f", estation_data->analog_ar_o3 * 0.01);
                                                 else size += sprintf(&(buf[size]), "null");
    }
    if(config_data.sensor_analog_soil_temperature != 0xFF){
        if(have_sensor) size += sprintf(&(buf[size]), ",");
        have_sensor = true;

        size += sprintf(&(buf[size]), "\"soil_t\":");
        if(estation_data->soil_temperature != 0xFFFFFFFF) size += sprintf(&(buf[size]), "%.2f", estation_data->soil_temperature * 0.01);
                                                     else size += sprintf(&(buf[size]), "null");
    }
    if(config_data.sensor_analog_soil_humidity != 0xFF){
        if(have_sensor) size += sprintf(&(buf[size]), ",");
        have_sensor = true;

        size += sprintf(&(buf[size]), "\"soil_h\":");
        if(estation_data->soil_humidity != 0xFFFFFFFF) size += sprintf(&(buf[size]), "%.2f", estation_data->soil_humidity * 0.01);
                                                  else size += sprintf(&(buf[size]), "null");
    }
    if(config_data.sensor_analog_uv_index != 0xFF){
        if(have_sensor) size += sprintf(&(buf[size]), ",");
        have_sensor = true;

        size += sprintf(&(buf[size]), "\"uv_index\":");
        if(estation_data->uv_index != 0xFFFFFFFF) size += sprintf(&(buf[size]), "%.2f", estation_data->uv_index * 0.01);
                                             else size += sprintf(&(buf[size]), "null");
    }
       
    size += sprintf(&(buf[size]), "}}");
    return size;
}



void estation_data_printf(){
    for(int32_t index=0;index < STORAGE_ADDRESS_DATA_N_ITEMS;index++){
        printf("%4d, %4x,", index, index);
        stored_data_read(index, &core_comm_estation_data);
        estation_data_json(&core_comm_estation_data, estation_data_buf_tmp);
        printf(estation_data_buf_tmp);
        printf("\r\n");
    }
}




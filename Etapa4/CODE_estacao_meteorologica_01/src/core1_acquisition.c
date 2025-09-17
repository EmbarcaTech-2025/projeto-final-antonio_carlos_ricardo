/**
 * @file    core1_acquisition.c
 * @author  Carlos e Ricardo
 * @brief   Inicializa os sensores e faz as aquisições
 * @version 0.1
 * @date    2025-09-16
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "hardware/flash.h"
#include "hardware/i2c.h"
#include "../lib/include/ad5592.h"
#include "../lib/include/aht10.h"
#include "../lib/include/bh1750.h"
#include "../lib/include/bmp280_main.h"
#include "../lib/include/hmc5883.h"
#include "../include/ad_board.h"
#include "../include/config.h"
#include "../include/config_data.h"
#include "../include/core_communication.h"
#include "../include/core1_acquisition.h"
#include "../include/core1_data.h"
#include "../include/estation_data.h"
#include "../include/rtc_wrap.h"
#include "../include/storage.h"
#include "../include/storage_adm.h"

// Fatores para conversão das grandezas analógicas
#define K_ANALOG_INT_CO         (   1.0 / 4096.0)
#define K_ANALOG_INT_CO2        (   1.0 / 4096.0)
#define K_ANALOG_INT_O3         (   1.0 / 4096.0)
#define K_ANALOG_INT_SOIL_TEMP  (   1.0 / 4096.0)
#define K_ANALOG_INT_SOIL_HUMI  (   1.0 / 4096.0)
#define K_ANALOG_INT_UV_INDEX   (1200.0 / 4096.0)

#define OFFSET_ANALOG_INT_CO         (0.0)
#define OFFSET_ANALOG_INT_CO2        (0.0)
#define OFFSET_ANALOG_INT_O3         (0.0)
#define OFFSET_ANALOG_INT_SOIL_TEMP  (0.0)
#define OFFSET_ANALOG_INT_SOIL_HUMI  (0.0)
#define OFFSET_ANALOG_INT_UV_INDEX   (0.0)


#define K_ANALOG_OUT_CO         (   1.0 / 4096.0)
#define K_ANALOG_OUT_CO2        (   1.0 / 4096.0)
#define K_ANALOG_OUT_O3         (   1.0 / 4096.0)
#define K_ANALOG_OUT_SOIL_TEMP  (   1.0 / 4096.0)
#define K_ANALOG_OUT_SOIL_HUMI  (   1.0 / 4096.0)
#define K_ANALOG_OUT_UV_INDEX   (1200.0 / 4096.0)

#define OFFSET_ANALOG_OUT_CO         (0.0)
#define OFFSET_ANALOG_OUT_CO2        (0.0)
#define OFFSET_ANALOG_OUT_O3         (0.0)
#define OFFSET_ANALOG_OUT_SOIL_TEMP  (0.0)
#define OFFSET_ANALOG_OUT_SOIL_HUMI  (0.0)
#define OFFSET_ANALOG_OUT_UV_INDEX   (0.0)


uint64_t next_flash_max_cycle;

/**
 * @brief Inicializa os sensores
 * 
 * @param next_cycle, número da próxima aquisição 
 */
void core1_acquisition_init(uint64_t next_cycle){
    next_flash_max_cycle = next_cycle;

    // Inicializa I2C sensors
    // rtc_ds3231 not need init
    if(config_data.sensor_i2c_pressure){
        bmp280_main_init();
    }
    if(config_data.sensor_i2c_humidity){
        aht10_init();
    }
    if(config_data.sensor_i2c_luminosity){
        bh1750_init();
    }
    if(config_data.sensor_i2c_wind_direction){
        hmc5883_init();
    }

    // Digital sensors
    if(config_data.sensor_digital_wind_speed){}
    if(config_data.sensor_digital_rain_intensity){}

    // Analog sensors
    bool ad_inside  = false;
    bool ad_outside = false;

    if(config_data.sensor_analog_ar_co            == 0) ad_inside = true;
    if(config_data.sensor_analog_ar_co2           == 0) ad_inside = true;
    if(config_data.sensor_analog_ar_o3            == 0) ad_inside = true;
    if(config_data.sensor_analog_soil_temperature == 0) ad_inside = true;
    if(config_data.sensor_analog_soil_humidity    == 0) ad_inside = true;
    if(config_data.sensor_analog_uv_index         == 0) ad_inside = true;

    if((config_data.sensor_analog_ar_co            > 0)&&(config_data.sensor_analog_ar_co            <= 8)) ad_outside = true;
    if((config_data.sensor_analog_ar_co2           > 0)&&(config_data.sensor_analog_ar_co2           <= 8)) ad_outside = true;
    if((config_data.sensor_analog_ar_o3            > 0)&&(config_data.sensor_analog_ar_o3            <= 8)) ad_outside = true;
    if((config_data.sensor_analog_soil_temperature > 0)&&(config_data.sensor_analog_soil_temperature <= 8)) ad_outside = true;
    if((config_data.sensor_analog_soil_humidity    > 0)&&(config_data.sensor_analog_soil_humidity    <= 8)) ad_outside = true;
    if((config_data.sensor_analog_uv_index         > 0)&&(config_data.sensor_analog_uv_index         <= 8)) ad_outside = true;

    if(ad_inside){}
    ad_boad_init(); // necessita para o Joystick independente do ad_inside

    if(ad_outside){
        ad5592_init();
    }
}

/**
 * @brief Executa a aquisição dos sensores e disponibiliza os resultados na variável: core1_estation_data
 * 
 */
void core1_acquisition_get(){
    // clear last read
    estation_data_clear(&core1_estation_data);

    core1_estation_data.cycle = next_flash_max_cycle;
    next_flash_max_cycle++;
    
    // get time
    core1_estation_data.t_aquire = rtc_wrap_get();

    // get I2C Sensors
    if(config_data.sensor_i2c_pressure){
        sensors_t bmp280 = bmp280_main_get_all_0();
        core1_estation_data.press  =           bmp280.pressure;
        core1_estation_data.temp_p = (int32_t)(bmp280.temperature * 100);
    }
    if(config_data.sensor_i2c_humidity){
        Aht10Data aht10 = aht10_read();
        core1_estation_data.humidity = (int32_t)(aht10.humidity * 100);
        core1_estation_data.temp_h   = (int32_t)(aht10.temp     * 100);
    }
    if(config_data.sensor_i2c_luminosity){
        core1_estation_data.luminos = bh1750_read_lux();
    }
    if(config_data.sensor_i2c_wind_direction){

    }

    // Digital sensors
    // Não implementados 


    uint8_t port;
    // Analog sensors
    if(        config_data.sensor_analog_ar_co            == 0){    // Analog sensor inside
        core1_estation_data.analog_ar_co     = (int32_t)((((float)ad_boad_get_value()) * K_ANALOG_INT_CO)        + OFFSET_ANALOG_INT_CO);
    } else if( config_data.sensor_analog_ar_co            <= 8){    // Analog sensor AD externo
        port = config_data.sensor_analog_ar_co - 1;
        core1_estation_data.analog_ar_co     = (int32_t)((((float)ad5592_get(port))    * K_ANALOG_OUT_CO)        + OFFSET_ANALOG_OUT_CO);

    } else if( config_data.sensor_analog_ar_co2           == 0){    // Analog sensor inside
        core1_estation_data.analog_ar_co2    = (int32_t)((((float)ad_boad_get_value()) * K_ANALOG_INT_CO2)       + OFFSET_ANALOG_INT_CO2);
    } else if( config_data.sensor_analog_ar_co2           <= 8){    // Analog sensor AD externo
        port = config_data.sensor_analog_ar_co2 - 1;
        core1_estation_data.analog_ar_co2    = (int32_t)((((float)ad5592_get(port))    * K_ANALOG_OUT_CO2)       + OFFSET_ANALOG_OUT_CO2);

    } else if( config_data.sensor_analog_ar_o3            == 0){    // Analog sensor inside
        core1_estation_data.analog_ar_o3     = (int32_t)((((float)ad_boad_get_value()) * K_ANALOG_INT_O3)        + OFFSET_ANALOG_INT_O3);
    } else if( config_data.sensor_analog_ar_o3            <= 8){    // Analog sensor AD externo
        port = config_data.sensor_analog_ar_o3 - 1;
        core1_estation_data.analog_ar_o3     = (int32_t)((((float)ad5592_get(port))    * K_ANALOG_OUT_O3)        + OFFSET_ANALOG_OUT_O3);

    } else if( config_data.sensor_analog_soil_temperature == 0){    // Analog sensor inside
        core1_estation_data.soil_temperature = (int32_t)((((float)ad_boad_get_value()) * K_ANALOG_INT_SOIL_TEMP) + OFFSET_ANALOG_INT_SOIL_TEMP);
    } else if( config_data.sensor_analog_soil_temperature <= 8){    // Analog sensor AD externo
        port = config_data.sensor_analog_soil_temperature - 1;
        core1_estation_data.soil_temperature = (int32_t)((((float)ad5592_get(port))    * K_ANALOG_OUT_SOIL_TEMP) + OFFSET_ANALOG_OUT_SOIL_TEMP);

    } else if( config_data.sensor_analog_soil_humidity    == 0){    // Analog sensor inside
        core1_estation_data.soil_humidity    = (int32_t)((((float)ad_boad_get_value()) * K_ANALOG_INT_SOIL_HUMI) + OFFSET_ANALOG_INT_SOIL_HUMI);
    } else if( config_data.sensor_analog_soil_humidity    <= 8){    // Analog sensor AD externo
        port = config_data.sensor_analog_soil_humidity - 1;
        core1_estation_data.soil_humidity    = (int32_t)((((float)ad5592_get(port))    * K_ANALOG_OUT_SOIL_HUMI) + OFFSET_ANALOG_OUT_SOIL_HUMI);

    } else if( config_data.sensor_analog_uv_index         == 0){    // Analog sensor inside
        core1_estation_data.uv_index         = (int32_t)((((float)ad_boad_get_value()) * K_ANALOG_INT_UV_INDEX)  + OFFSET_ANALOG_INT_UV_INDEX);
    } else if( config_data.sensor_analog_uv_index         <= 8){    // Analog sensor AD externo
        port = config_data.sensor_analog_uv_index - 1;
        core1_estation_data.uv_index         = (int32_t)((((float)ad5592_get(port))    * K_ANALOG_OUT_UV_INDEX)  + OFFSET_ANALOG_OUT_UV_INDEX);
    }
}
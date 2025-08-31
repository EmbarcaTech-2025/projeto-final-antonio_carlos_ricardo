#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "hardware/flash.h"
#include "hardware/i2c.h"
#include "../lib/include/aht10.h"
#include "../lib/include/bh1750.h"
#include "../lib/include/bmp280_main.h"
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

uint64_t flash_max_cycle;
int32_t  flash_index;

uint64_t next_flash_max_cycle;
int32_t  flash_index;
int32_t  next_flash_index;

void core1_acquisition_init(uint64_t cycle){
    // I2C init,  init I2C Main
    /*
    i2c_init(         I2C_MAIN_BUS, 400 * 1000);
    gpio_pull_up(     I2C_MAIN_GPIO_SDA);
    gpio_pull_up(     I2C_MAIN_GPIO_SCL);
    gpio_set_function(I2C_MAIN_GPIO_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_MAIN_GPIO_SCL, GPIO_FUNC_I2C);
    */

    // I2C sensors
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
    if(config_data.sensor_i2c_wind_direction){}

    // Digital sensors
    if(config_data.sensor_digital_wind_speed){}
    if(config_data.sensor_digital_rain_intensity){}


    // Analog sensors
    ad_boad_init();

    
    bool ad_inside  = false;
    bool ad_outside = false;

    if(config_data.sensor_analog_ar_co            == 8) ad_inside = true;
    if(config_data.sensor_analog_ar_co2           == 8) ad_inside = true;
    if(config_data.sensor_analog_ar_o3            == 8) ad_inside = true;
    if(config_data.sensor_analog_soil_temperature == 8) ad_inside = true;
    if(config_data.sensor_analog_soil_humidity    == 8) ad_inside = true;
    if(config_data.sensor_analog_uv_index         == 8) ad_inside = true;

    if(config_data.sensor_analog_ar_co            < 8) ad_outside = true;
    if(config_data.sensor_analog_ar_co2           < 8) ad_outside = true;
    if(config_data.sensor_analog_ar_o3            < 8) ad_outside = true;
    if(config_data.sensor_analog_soil_temperature < 8) ad_outside = true;
    if(config_data.sensor_analog_soil_humidity    < 8) ad_outside = true;
    if(config_data.sensor_analog_uv_index         < 8) ad_outside = true;

    if(ad_inside){}
    if(ad_outside){}


    if(cycle == 0xFFFFFFFFFFFFFFFF){
        cycle = 0;
    }else{
        cycle += config_data_get_period_in_sec(config_data.period_acquisition);
    }
    next_flash_max_cycle = cycle;

    /*
    uint64_t flash_max_cycle;
    bool ha_dados = stored_data_get_last_write_index(&flash_max_cycle, &flash_index);
    if(ha_dados){
        next_flash_max_cycle = flash_max_cycle + config_data_get_period_in_sec(config_data.period_acquisition);
        next_flash_index     = flash_index + 1;
    }else{
        next_flash_max_cycle = 0;
        next_flash_index     = 0;
    }
    printf("Ha Dados:%s, flash_index=%d, flash_max_cycle=%llu, next_flash_index=%d, next_flash_max_cycle=%llu\n", ha_dados?"True":"False", flash_index, flash_max_cycle, next_flash_index, next_flash_max_cycle);
    */
}
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
    if(config_data.sensor_i2c_wind_direction){}

    // Digital sensors

    // Analog sensors

    bool sensor_i2c_pressure;
    bool sensor_i2c_humidity;
    bool sensor_i2c_luminosity;
    bool sensor_i2c_wind_direction;

    bool sensor_digital_wind_speed;
    bool sensor_digital_rain_intensity;

    uint8_t sensor_analog_ar_co;
    uint8_t sensor_analog_ar_co2;
    uint8_t sensor_analog_ar_o3;
    uint8_t sensor_analog_soil_temperature;
    uint8_t sensor_analog_soil_humidity;
    uint8_t sensor_analog_uv_index;
}



// ---------- Função de escrita na flash (em RAM) ----------
static void __not_in_flash_func(core1_acquisition_write_data_on_flash)(EstationData * estation_data, uint32_t index, bool erase) {
    uint32_t ints = save_and_disable_interrupts();

    if((!(index & 0x000F))&&(erase)) {
        flash_range_erase(STORAGE_OFFSET_DATA_FIRST + ((index >> 4) * STORAGE_CLEAR_BLOCK_SIZE), STORAGE_CLEAR_BLOCK_SIZE);
    }
    flash_range_program(STORAGE_OFFSET_DATA_FIRST + (index * STORAGE_ADDRESS_DATA_SIZE), (uint8_t *)estation_data, sizeof(EstationData));

    restore_interrupts(ints);
}


static void core1_acquisition_save(EstationData * estation_data, uint32_t index, bool erase){
    core_comm_flash_busy = true;
    core_comm_core0_busy = true;

    printf("SAVING Index=%d\n", index);

    // pede para parar o core 0
    multicore_fifo_push_blocking(CORE_COMM_CMD_HOLD);

    // aguarda parada do core0
    while(core_comm_core0_busy);

    // core 0 parou, poso executar

    core1_acquisition_write_data_on_flash(estation_data, index, erase);


    // avisa o core 0 que operação com flash acabou
    core_comm_flash_busy = false;

    // aguarda o core 0 voltar a rodar
    while(!core_comm_core0_busy);
}

void core1_acquisition_save_aq(){
    int32_t index = storage_adm_get_next_index();
    core1_acquisition_save(&core1_estation_data, index, true);
}
void core1_acquisition_update(EstationData * estation_data, uint32_t index){
    core1_acquisition_save(estation_data, index, false);
}

int32_t core1_acquisition_get_next_send_index(){
    return -1;

}
void    core1_acquisition_sent_last_index(){

}

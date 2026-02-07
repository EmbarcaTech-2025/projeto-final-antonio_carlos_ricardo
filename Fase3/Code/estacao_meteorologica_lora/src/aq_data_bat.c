#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "../include/aq_data_bat.h"
#include "../include/code_config.h"
#include "../include/loop_printf.h"

#define INA226_ADDR 0x40    // Endereço padrão do INA226

// Definir registradores
#define CONFIG_REG          0x00
#define SHUNT_VOLTAGE_REG   0x01
#define BUS_VOLTAGE_REG     0x02
#define POWER_REG           0x03
#define CURRENT_REG         0x04
#define CALIBRATION_REG     0x05

int aqdatabat_init_power_on(){
    return 0;
}
int aqdatabat_init_aq(){
    return 0;
}
int aqdatabat_read(AqDataBat_Value *value){
    int erro = 0;
    int ret;

    value->bat_level = 0xFF;
    
    uint8_t config[] = {CONFIG_REG, 0x41, 0x27};    // Configuração: medir corrente, potência e tensão (exemplo)
    if(!erro){
        ret = i2c_write_timeout_us(I2C_MAIN_BUS, INA226_ADDR, config, 3, false, 2000);  //25us por item
        if(ret != 3) erro = 1;
    }
    
    // voltage
    uint8_t data_v[2];
    uint8_t reg[] = {BUS_VOLTAGE_REG};

    if(!erro){
        ret = i2c_write_timeout_us(I2C_MAIN_BUS, INA226_ADDR, reg, 1, false, 2000);  //25us por item
        if(ret != 1) erro = 2;
    }
    if(!erro){
        i2c_read_blocking(I2C_MAIN_BUS, INA226_ADDR, data_v, 2, false);
        ret = i2c_read_timeout_us( I2C_MAIN_BUS, INA226_ADDR, data_v, 2, false, 2000);
        if(ret != 2) erro = 3;
    }

    if(erro){
        if(DEBUG_ON_BAT){
            loop_printf("V Bat ERROR = %d\n", erro);
        }
        return erro; 
    }

    int v = ((((uint32_t)data_v[0]) * 256) + data_v[1]) * 1.25;
    if(DEBUG_ON_BAT){
        loop_printf("V Bat = %.3f Volts\n", v * 0.001);
    }

    if(v > BAT_MAX_MV) v = BAT_MAX_MV;
    if(v < BAT_MIN_MV) v = BAT_MIN_MV;
    value->bat_level = 200.0 * (v - BAT_MIN_MV)/ (BAT_MAX_MV - BAT_MIN_MV);
    if(DEBUG_ON_BAT){
        loop_printf("Bat Level = %.1f %%\n", value->bat_level * 0.5);
    }
    
    return 0;
}
int aqdatabat_sleep(){
    return 0;
}


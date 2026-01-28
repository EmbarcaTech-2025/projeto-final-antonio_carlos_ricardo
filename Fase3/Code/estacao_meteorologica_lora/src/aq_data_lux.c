#include "hardware/i2c.h"
#include "../include/aq_data_lux.h"
#include "../include/code_config.h"
#include "../include/loop_printf.h"



#define BH1750_ADDR             0x23 // ou 0x5C, dependendo do sensor

#define BH1750_CMD_POWER_DOWN   0x00
#define BH1750_CMD_POWER_ON     0x01
#define BH1750_CMD_RESET        0x07

#define BH1750_CMD_HRES_CONT    0x10    // Modo contínuo, alta resolução, passo da leitura é 1 em 1
#define BH1750_CMD_LRES_CONT    0x13    // Start measurement at 4lx resolution. Measurement Time is typically 16ms.
                                        // passo da leitura é 4 em 4, "mesma" leitura raw do BH1750_CMD_HRES_CONT
#define BH1750_CMD_HRES2_CONT   0x11    // passo da leitura é 1 em 0,5, ou seja, o raw mostra o dobro do BH1750_CMD_HRES_CONT

#define BH1750_CMD_TIME_HIGH    0x40    // 01000 [7,6,5]
#define BH1750_CMD_TIME_LOW     0x60    // 011   [4,3,2,1,0]

#define BH1750_MTREG           31 // 69  // 0x31    // default = 0x69


static uint16_t aqdatalux_lux_k_10000;







int aqdatalux_init_power_on(uint16_t lux_k_10000){
    uint8_t cmd;

    aqdatalux_lux_k_10000 = lux_k_10000;

    // Ligar
    cmd = BH1750_CMD_POWER_ON;
    if(i2c_write_timeout_us(I2C_MAIN_BUS, BH1750_ADDR, &cmd, 1, false, I2C_TIMEOUT_US_BH1750) != 1) return -1;
    sleep_us(2000);

    // Reset
    cmd = BH1750_CMD_RESET;
    if(i2c_write_timeout_us(I2C_MAIN_BUS, BH1750_ADDR, &cmd, 1, false, I2C_TIMEOUT_US_BH1750) != 1) return -2;
    sleep_us(2000);

    // Define fator:
    uint8_t cmd1 = 0x40 | (BH1750_MTREG >> 5);      // bits [7:5]
    uint8_t cmd2 = 0x60 | (BH1750_MTREG & 0x1F);    // bits [4:0]

    if(i2c_write_timeout_us(I2C_MAIN_BUS, BH1750_ADDR, &cmd1, 1, false, I2C_TIMEOUT_US_BH1750) != 1) return -3;
    if(i2c_write_timeout_us(I2C_MAIN_BUS, BH1750_ADDR, &cmd2, 1, false, I2C_TIMEOUT_US_BH1750) != 1) return -4;


    // Set mode
    cmd = BH1750_CMD_HRES_CONT;
    if(i2c_write_timeout_us(I2C_MAIN_BUS, BH1750_ADDR, &cmd, 1, false, I2C_TIMEOUT_US_BH1750) != 1) return -5;
    sleep_us(20000);

    return 0;
}
int aqdatalux_init_aq(){
    //bh1750_init();
    return 0;
}
int aqdatalux_read(AqDataLux_Value *value){
    uint8_t data[2];
    if(i2c_read_timeout_us(I2C_MAIN_BUS, BH1750_ADDR, data, 2, false, I2C_TIMEOUT_US_BH1750) == 2){
        uint64_t raw = (data[0] << 8) | data[1];

//        uint32_t rawh, rawl;

//        rawh = (uint32_t)(raw >> 32);
//        rawl = (uint32_t)(raw & 0x0000FFFF);
//        printf("1-RawH=0x%08x, RawL=0x%08x, RawL=%d\n", rawh, rawl, rawl);

        if(raw == 0xFFFF){
            // over flow
            value->lux_level_x4 = 0xFFFF;
        }else{
            raw = (raw * (uint64_t)aqdatalux_lux_k_10000 * 69) / BH1750_MTREG / 12000;

//            rawh = (uint32_t)(raw >> 32);
//            rawl = (uint32_t)(raw & 0x0000FFFF);
//            printf("2-RawH=0x%08x, RawL=0x%08x, RawL=%d\n", rawh, rawl, rawl);

            raw >>= 2;
            if(raw >= 0x00FFFF) value->lux_level_x4 = 0xFFFF;
                           else value->lux_level_x4 = (uint16_t)raw;

//            rawh = (uint32_t)(raw >> 32);
//            rawl = (uint32_t)(raw & 0x0000FFFF);
//            printf("3-RawH=0x%08x, RawL=0x%08x, RawL=%d\n", rawh, rawl, rawl);
        }

        if(DEBUG_ON_LUXIMETER){
            loop_printf("Luximeter       = %d lux\n", (uint32_t)value->lux_level_x4 * 4);
        }
        return 0;
    }else{
        return -1;
    }
}

int aqdatalux_sleep(){
    return 0;
}
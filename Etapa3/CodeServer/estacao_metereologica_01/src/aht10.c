#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "../include/aht10.h"
#include "../include/config.h"

#define AHTX0_I2CADDR_DEFAULT   0x38   ///< AHT default i2c address
#define AHTX0_I2CADDR_ALTERNATE 0x39 ///< AHT alternate i2c address
#define AHTX0_CMD_CALIBRATE 0xE1     ///< Calibration command
#define AHTX0_CMD_TRIGGER 0xAC       ///< Trigger reading command
#define AHTX0_CMD_SOFTRESET 0xBA     ///< Soft reset command
#define AHTX0_STATUS_BUSY 0x80       ///< Status bit for busy
#define AHTX0_STATUS_CALIBRATED 0x08 ///< Status bit for calibrated

#define ATH10_ADDRESS AHTX0_I2CADDR_DEFAULT


void aht10_init(){
    uint8_t buffer[1];

    uint8_t cmd = AHTX0_CMD_SOFTRESET; // Inicializa
    i2c_write_blocking(I2C_MAIN_BUS, ATH10_ADDRESS, &cmd, 1, false);

    bool fim = false;
    while(!fim){
        i2c_read_blocking(I2C_MAIN_BUS, ATH10_ADDRESS, buffer, 1, false);
        if(buffer[0] & AHTX0_STATUS_BUSY){
            sleep_ms(10);
        }else{
            fim = true;
        }
    }
}


Aht10Data aht10_read(){
    Aht10Data ret;
    uint8_t buffer[6];

    // Trigger
    buffer[0] = 0xAC;    // Trigger
    buffer[1] = 0x33;
    buffer[2] = 0x00;
    i2c_write_blocking(I2C_MAIN_BUS, ATH10_ADDRESS, buffer, 3, false);
    //printf("Trigger\n");

    // aguarda leitura
    uint32_t t0 = time_us_32();
    bool fim = false;
    while(!fim){
        i2c_read_blocking(I2C_MAIN_BUS, ATH10_ADDRESS, buffer, 1, false);
        if(buffer[0] & AHTX0_STATUS_BUSY){
            //printf("busy\n");
        }else{
            fim = true;
        }
    }
    // printf("DeltaT=%d us\n", time_us_32() - t0); tempo em torno de 35ms


    // ler dados
    i2c_read_blocking(I2C_MAIN_BUS, ATH10_ADDRESS, buffer, 6, false);

    int32_t h, t;
    h = (buffer[1] << 12) | (buffer[2] << 4) | ((buffer[3] >> 4) & 0x0F);
    t = ((buffer[3] & 0x0F) << 16) | (buffer[4] << 8) | buffer[5];

    ret.humidity = (h * 100.0) / 0x100000, 
    ret.temp     = ((t * 200.0)/0x100000) - 50.0;

    //for(int i=0;i<6;i++){ printf("0x%02x ", buffer[i]);}

    //printf("H=0x%06X, T=0x%06X,  ", h, t);
    //printf("H=%f, T=%f\n", (h * 100.0) / 0x100000, ((t *200)/0x100000) - 50.0);

    return ret;
}



#ifndef AD_DATA_BMEP280_H
#define AD_DATA_BMEP280_H

#include "pico/stdlib.h"





typedef enum{
    AQDATABMEP280_DEV_ERROR = 0,
    AQDATABMEP280_DEV_BMP280,
    AQDATABMEP280_DEV_BME280
} AqDataBmep280_Device;

typedef struct{
    uint8_t  humidity;
    int16_t  temp;
    uint16_t press;
} AqDataBmep280_Value_I;

int aqdatabmep280_init_power_on();
int aqdatabmep280_init_aq();
int aqdatabmep280_read_i(AqDataBmep280_Value_I *value);
int aqdatabmep280_sleep();


void aqdatabmep280_teste();

#endif // AD_DATA_BMEP280_H
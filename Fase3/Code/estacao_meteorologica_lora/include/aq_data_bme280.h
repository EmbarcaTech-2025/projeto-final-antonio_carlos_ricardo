#ifndef AD_DATA_BME280_H
#define AD_DATA_BME280_H

#include "pico/stdlib.h"

typedef struct{
    uint8_t value;

} AqDataBme280_Value;


int aqdatabme280_init();
int aqdatabme280_read(AqDataBme280_Value *value);
int aqdatabme280_sleep();

#endif // AD_DATA_BME280_H
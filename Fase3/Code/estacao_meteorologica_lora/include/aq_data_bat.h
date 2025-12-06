#ifndef AQ_DATA_BAT_H
#define AQ_DATA_BAT_H

#include "pico/stdlib.h"

typedef struct{
    uint8_t bat_level;

} AqDataBat_Value;

int aqdatabat_init();
int aqdatabat_read(AqDataBat_Value *value);
int aqdatabat_sleep();


#endif // AQ_DATA_BAT_H
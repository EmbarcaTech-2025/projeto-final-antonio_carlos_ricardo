#ifndef AQ_DATA_BAT_H
#define AQ_DATA_BAT_H

#include "pico/stdlib.h"

typedef struct{
    uint8_t bat_level;

} AqDataBat_Value;

#define BAT_MAX_MV        4200     // 100%
#define BAT_MIN_MV        3300     //   0%

int aqdatabat_init_power_on();
int aqdatabat_init_aq();
int aqdatabat_read(AqDataBat_Value *value);
int aqdatabat_sleep();


#endif // AQ_DATA_BAT_H
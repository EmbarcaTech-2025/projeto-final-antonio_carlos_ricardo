#ifndef AQ_DATA_LUX_H
#define AQ_DATA_LUX_H

#include "pico/stdlib.h"

typedef struct{
    uint16_t lux_level;

} AqDataLux_Value;

int aqdatalux_init();
int aqdatalux_read(AqDataLux_Value *value);
int aqdatalux_sleep();


#endif // AQ_DATA_LUX_H
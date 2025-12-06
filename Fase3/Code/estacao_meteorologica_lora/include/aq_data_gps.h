#ifndef AD_DATA_GPS_H
#define AD_DATA_GPS_H

#include "pico/stdlib.h"

typedef struct{
    uint8_t value;

} AqDataGps_Value;

int aqdatagps_init();
int aqdatagps_read(AqDataGps_Value *value);
int aqdatagps_sleep();


#endif // AD_DATA_GPS_H
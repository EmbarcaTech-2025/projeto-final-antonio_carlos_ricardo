#ifndef AQ_DATA_H
#define AQ_DATA_H

#include "pico/stdlib.h"
#include "aq_data_bat.h"
#include "aq_data_bme280.h"
#include "aq_data_gps.h"


typedef enum{
    AQDATA_CONTROL1_BAT_VALUE = 0x01,
    AQDATA_CONTROL1_BME280    = 0x02,
    AQDATA_CONTROL1_GPS       = 0x04,
    // reservados para futuros:
    AQDATA_CONTROL1_LUX       = 0x08,
    AQDATA_CONTROL1_UV        = 0x10,
    AQDATA_CONTROL1_WIND      = 0x20,
    AQDATA_CONTROL1_RAIN      = 0x40,
    AQDATA_CONTROL1_TBD       = 0x80,
} AqData_Control1;


typedef struct{
    uint8_t             control1;
    AqDataBat_Value     battery;
    AqDataBme280_Value  bme280;
    AqDataGps_Value     gps;
    // add futuros
} AqData;


int aqdata_init( AqData *value);
int aqdata_read( AqData *value);
int aqdata_sleep(AqData *value);

#endif // AQ_DATA_H
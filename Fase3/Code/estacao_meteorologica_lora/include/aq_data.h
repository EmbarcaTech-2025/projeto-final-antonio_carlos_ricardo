#ifndef AQ_DATA_H
#define AQ_DATA_H

#include "pico/stdlib.h"
#include "aq_data_ad.h"
#include "aq_data_bat.h"
#include "aq_data_bmep280.h"
#include "aq_data_gps.h"
#include "aq_data_lux.h"
#include "est_config.h"

typedef enum{
    AQ_ITEM_BAT_VALUE = 0x01,
    AQ_ITEM_BME280    = 0x02,
    AQ_ITEM_GPS       = 0x04,
    AQ_ITEM_LUX       = 0x08,
    // reservados para futuros:
    AQ_ITEM_UV        = 0x10,
    AQ_ITEM_WIND      = 0x20,
    AQ_ITEM_VSYS      = 0x40,
    AQ_ITEM_CPU_TEMP  = 0x80
} AqItem;


typedef struct{
    ActiveSensors         active_sensors;
    AqDataBat_Value       battery;
    AqDataBmep280_Value_I bmep280;
    AqDataGps_Value       gps;
    AqDataLux_Value       lux;
    uint8_t               vsys;
    int16_t               cpu_temp_deci;
    // add futuros
} AqData;

int aqdata_init_power_on(AqData *value, uint16_t lux_k_10000, uint16_t vsys_k_10000);
int aqdata_init_aq(      AqData *value);
int aqdata_read(         AqData *value);
int aqdata_sleep(        AqData *value);

#endif // AQ_DATA_H
#include "../include/aq_data.h"

int aqdata_init( AqData *value){
    if(value == NULL) return 0xFFFF;
    int ret = 0;
    if(value->control1  & AQDATA_CONTROL1_BAT_VALUE){
        if(aqdatabat_init()) ret |= AQDATA_CONTROL1_BAT_VALUE;
    }
    if(value->control1  & AQDATA_CONTROL1_BME280){
        if(aqdatabme280_init()) ret |= AQDATA_CONTROL1_BME280;
    }
    if(value->control1  & AQDATA_CONTROL1_GPS){
        if(aqdatagps_init()) ret |= AQDATA_CONTROL1_GPS;
    }
    return ret;
}
int aqdata_read( AqData *value){
    if(value == NULL) return 0xFFFF;
    int ret = 0;
    if(value->control1  & AQDATA_CONTROL1_BAT_VALUE){
        if(aqdatabat_read(&value->battery)) ret |= AQDATA_CONTROL1_BAT_VALUE;
    }
    if(value->control1  & AQDATA_CONTROL1_BME280){
        if(aqdatabme280_read(&value->bme280)) ret |= AQDATA_CONTROL1_BME280;
    }
    if(value->control1  & AQDATA_CONTROL1_GPS){
        if(aqdatagps_read(&value->gps)) ret |= AQDATA_CONTROL1_GPS;
    }
    return ret;
}
int aqdata_sleep(AqData *value){
        if(value == NULL) return 0xFFFF;
    int ret = 0;
    if(value->control1  & AQDATA_CONTROL1_BAT_VALUE){
        if(aqdatabat_sleep()) ret |= AQDATA_CONTROL1_BAT_VALUE;
    }
    if(value->control1  & AQDATA_CONTROL1_BME280){
        if(aqdatabme280_sleep()) ret |= AQDATA_CONTROL1_BME280;
    }
    if(value->control1  & AQDATA_CONTROL1_GPS){
        if(aqdatagps_sleep()) ret |= AQDATA_CONTROL1_GPS;
    }
    return ret;
}
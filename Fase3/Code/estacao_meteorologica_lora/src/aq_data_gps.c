#include "../include/aq_data_gps.h"
#include "gps/gy_neo6mv2.h"

int aqdatagps_init_power_on(){
    return 0;
}
int aqdatagps_init_aq(){
    gps_init();
    return 0;
}
int aqdatagps_read(AqDataGps_Value *value){
    int32_t lat, lon, alt;

    uint64_t t0 = time_us_64() + 10000000;
    GpsGgaError ret;
    bool fim = false;
    while(!fim){
        ret = gps_read(&lat, &lon, &alt);
        if(!ret) fim = true;
        else{
            if(time_us_64() > t0) fim = true;
        }

    }
    if(ret){
        value->altitude  = 0x7FFF;
        value->latitude  = 0x7FFFFFFF;
        value->longitude = 0x7FFFFFFF;
    }else{
        value->altitude  = alt / 6553.6;
        value->latitude  = lat;
        value->longitude = lon;
    }
    
    return 0;
}
int aqdatagps_sleep(){
    return 0;
}
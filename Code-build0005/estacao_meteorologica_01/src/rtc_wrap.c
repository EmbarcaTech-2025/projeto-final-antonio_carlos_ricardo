#include "pico/stdlib.h"
#include "../include/config_data.h"
#include "../include/rtc_wrap.h"
#include "../lib/include/rtc_ds3231.h"


uint64_t t0 = 0;
uint64_t timestamp0 = 1756454400;

uint64_t rtc_wrap_get(){
    if(config_data.rtc_with) return rtc_ds3231_get();

    return timestamp0 + ((time_us_64() - t0) / 1000000);
}

void     rtc_wrap_set(uint64_t timestamp){
    if(config_data.rtc_with) return rtc_ds3231_set(timestamp);
    t0         = time_us_64(); 
    timestamp0 = timestamp;
}
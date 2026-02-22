/**
 * @file    rtc_wrap.c
 * @author  Carlos e Ricardo
 * @brief   Lê e atualiza o Clock que pode ser um RTC físico ou um virtual
 * @version 0.1
 * @date    2025-09-05
 * 
 * @copyright Copyright (c) 2025
 */
#include "pico/stdlib.h"
#include "../include/config_data.h"
#include "../include/rtc_wrap.h"
#include "../lib/include/rtc_ds3231.h"

// valores utilizados pelo RTC Virtual
uint64_t t0         = 0;
uint64_t timestamp0 = 1756454400;   // 2025-08-29 08:00

/**
 * @brief Retorna o timestamp atual do Clock
 * 
 * @return uint64_t, timestamp atual do Clock
 */
uint64_t rtc_wrap_get(){
    if(config_data.rtc_with) return rtc_ds3231_get();

    return timestamp0 + ((time_us_64() - t0) / 1000000);
}

/**
 * @brief seta o timestamp atual do Clock
 * 
 * @param timestamp, valor para o set do timestamp atual do Clock
 */
void     rtc_wrap_set(uint64_t timestamp){
    if(config_data.rtc_with) return rtc_ds3231_set(timestamp);
    t0         = time_us_64(); 
    timestamp0 = timestamp;
}
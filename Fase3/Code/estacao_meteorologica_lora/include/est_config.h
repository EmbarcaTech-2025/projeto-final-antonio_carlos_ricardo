#ifndef EST_CONFIG_H
#define EST_CONFIG_H

#include "pico/stdlib.h"

typedef struct {
    uint32_t device_address;
    uint8_t  app_s_key[16];
    uint8_t  net_s_key[16];
}LoraAbpPar;

typedef struct {
    uint32_t device_address;
    uint8_t  app_s_key[16];
    uint8_t  net_s_key[16];
}LoraOtaaPar;


typedef struct {
    bool        mode_otaa;      // true ==> OTAA, false ==> ABP
    LoraAbpPar  lora_abp_par;
    LoraOtaaPar lora_otaa_par;
    uint8_t     sensors1;       // AqData_Control1
    uint16_t    sleep_time_min; // Aquisition period in minutes
}EstConfig;

void est_config_default(EstConfig * est_config);

//extern EstConfig est_config;


#endif // EST_CONFIG_H

#include "../include/est_config.h"



static void lora_abp_par_default(LoraAbpPar * lora_abp_par){
    /*
    uint32_t device_address;
    uint8_t  app_s_key[16];
    uint8_t  net_s_key[16];
    */
}

static void lora_otaa_par_default(LoraOtaaPar * lora_otaa_par){
    /*
        uint32_t device_address;
    uint8_t  app_s_key[16];
    uint8_t  net_s_key[16];*/
}

void est_config_default(EstConfig * est_config){
    est_config->mode_otaa = false;      // ABP
    lora_abp_par_default( &est_config->lora_abp_par);
    lora_otaa_par_default(&est_config->lora_otaa_par);
    est_config->sensors1 = 0x00;        // No Sensors
    est_config->sleep_time_min = 5;     // 5 minutes
}
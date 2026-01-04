#include "../include/est_config.h"



static void lora_abp_par_default(LoraAbpPar * lora_abp_par){
    lora_abp_par->channel        = 5;
    lora_abp_par->sf             = 7;
    lora_abp_par->device_address = 0x12345678;
    lora_abp_par->fcnt           = 0x0123;

    uint8_t app[] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};
    uint8_t net[] = {0xFF, 0xEE, 0xDD, 0xCC, 0xBB, 0xAA, 0x99, 0x88, 0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11, 0x00};
    
    for(int i=0;i<16;i++){
        lora_abp_par->app_s_key[i] = app[i];
        lora_abp_par->net_s_key[i] = net[i];
    }
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
    //est_config->sensors1 = 0x00;        // No Sensors
    est_config->sensors1 = 0x07;        // No Sensors
    est_config->sleep_time_min = 5;     // 5 minutes
}
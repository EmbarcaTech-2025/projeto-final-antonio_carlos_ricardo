#include <string.h>
#include "../include/est_config.h"

const uint8_t dev_add[4]    = {0x12, 0x34, 0x56, 0x78};

const uint8_t app_s_key[16] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};     // AppSKey	16 bytes	Chave da aplicação
const uint8_t nwk_s_key[16] = {0xFF, 0xEE, 0xDD, 0xCC, 0xBB, 0xAA, 0x99, 0x88, 0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11, 0x00};     // NwkSKey	16 bytes	Chave de rede

const uint8_t dev_eui[8]    = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};                                                       // DevEUI	              8 bytes	Identificador único do dispositivo
const uint8_t app_eui[8]    = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77};                                                       // AppEUI (ou JoinEUI)  8 bytes	Identificador da aplicação
const uint8_t app_key[16]   = {0xFF, 0xEE, 0xDD, 0xCC, 0xBB, 0xAA, 0x99, 0x88, 0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11, 0x00};       // AppKey	             16 bytes	Chave secreta (AES-128)


void est_config_default(EstConfig * est_config){
    est_config->lora_mode = LORA_MODE_LORAWAN_ABP;

    est_config->lora_par.channel        = 5;
    est_config->lora_par.sf             = 7;
    memcpy(est_config->lora_par.device_address, dev_add, 4);

    est_config->lorawan_abp_par.channel        = 5 ;
    est_config->lorawan_abp_par.sf             = 7;
    est_config->lorawan_abp_par.fcnt           = 0x0123;
    memcpy(est_config->lorawan_abp_par.device_address, dev_add,    4);
    memcpy(est_config->lorawan_abp_par.app_s_key,      app_s_key, 16);   // AppSKey	16 bytes	Chave da aplicação
    memcpy(est_config->lorawan_abp_par.nwk_s_key,      nwk_s_key, 16);   // NwkSKey	16 bytes	Chave de r

    est_config->lorawan_otaa_par.channel = 5;
    est_config->lorawan_otaa_par.sf      = 7;
    memcpy(est_config->lorawan_otaa_par.dev_eui, dev_eui, 8);       // DevEUI	             8 bytes	Identificador único do dispositivo
    memcpy(est_config->lorawan_otaa_par.app_eui, app_eui, 8);       // AppEUI (ou JoinEUI)   8 bytes	Identificador da aplicação
    memcpy(est_config->lorawan_otaa_par.app_key, app_key, 16);      // AppKey	            16 bytes	Chave secreta (AES-128)

    est_config->sleep_time_min = 0;     // teste a cada 10 segundos


    est_config->active_sensors.battery = true;
    est_config->active_sensors.bme280  = true;
    est_config->active_sensors.gps     = false;
    est_config->active_sensors.lux     = false;
    est_config->active_sensors.uv      = false;   // not implemented
    est_config->active_sensors.wind    = false;   // not implemented
    est_config->active_sensors.rain    = false;   // not implemented
    est_config->active_sensors.tbd     = false;   // not implemented

    est_config->usb_mode = USB_MODE_OFF;
    est_config->leds_on  = false;
}
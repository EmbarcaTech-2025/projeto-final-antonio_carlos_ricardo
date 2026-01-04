#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "../include/aq_data.h"
#include "../include/code_config.h"
#include "../include/est_config.h"
#include "../include/flash.h"
#include "../include/menu_conf.h"


void menu_conf(EstConfig * est_config){
    bool fim = false;
    while(!fim){
        printf("\033[2J\033[H");    // Faz com que o terminal volte para o início da tela

        printf("********** %s **********\n\n", NAME);
        printf("Version: %s - %s - Build: %s\n\n\n", VERSION, VERSION_DATA, BUILD);
        printf("M) LoRaWAN Mode: %s\n\n", est_config->mode_otaa?"OTAA":"ABP");

        if(est_config->mode_otaa){
            printf("----- OTAA Parameters ----\n");
            printf("D) Device EUI     : \n");
            printf("A) Application Key: \n");
            printf("\n\n");
        }else{
            printf("----- ABP Parameters ----\n");
            printf("-) Channel                 : %02d\n", est_config->lora_abp_par.channel);
            printf("-) SF                      : %02d\n", est_config->lora_abp_par.sf);
            printf("D) Device address          : 0x%08X\n", est_config->lora_abp_par.device_address);
            printf("A) Application Session Key : 0x");
            for(int i=0;i<16;i++) printf(" %02X", est_config->lora_abp_par.app_s_key[i]);
            printf("\nN) Network     Session Key : 0x");
            for(int i=0;i<16;i++) printf(" %02X", est_config->lora_abp_par.net_s_key[i]);
            printf("\nF) Frame Start counter     : 0x%04X\n", est_config->lora_abp_par.fcnt);
        }
        printf("\n\nU) Update time: %d minutes\n\n", est_config->sleep_time_min);

        printf("1) Battery level: %s\n", est_config->sensors1 & AQDATA_CONTROL1_BAT_VALUE?"On":"Off");
        printf("2) BME280       : %s\n", est_config->sensors1 & AQDATA_CONTROL1_BME280?   "On":"Off");
        printf("3) GPS          : %s\n", est_config->sensors1 & AQDATA_CONTROL1_GPS?      "On":"Off");
        printf("4) Luximeter    : %s\n", est_config->sensors1 & AQDATA_CONTROL1_LUX?      "On":"Off");
        printf("5) UV Index     : %s\n", est_config->sensors1 & AQDATA_CONTROL1_UV?       "On":"Off");
        printf("6) Wind         : %s\n", est_config->sensors1 & AQDATA_CONTROL1_WIND?     "On":"Off");
        printf("7) Rain         : %s\n", est_config->sensors1 & AQDATA_CONTROL1_RAIN?     "On":"Off");
        printf("8) TBD          : %s\n", est_config->sensors1 & AQDATA_CONTROL1_TBD?      "On":"Off");

        printf("\nC) Clear all data\n");
        printf("R) Reset Configuration\n");
        printf("\nW) Write configuration\n");
        printf("\nX) eXit");

        switch(toupper(getchar())){
            case 'M': est_config->mode_otaa = !est_config->mode_otaa; break;
            case 'U': 
                switch(est_config->sleep_time_min){
                    case  1: est_config->sleep_time_min =  2; break;
                    case  2: est_config->sleep_time_min =  5; break;
                    case  5: est_config->sleep_time_min = 10; break;
                    case 10: est_config->sleep_time_min = 15; break;
                    case 15: est_config->sleep_time_min = 20; break;
                    case 20: est_config->sleep_time_min = 30; break;
                    case 30: est_config->sleep_time_min = 45; break;
                    case 45: est_config->sleep_time_min = 60; break;
                    case 60: est_config->sleep_time_min =  1; break;
                    default: est_config->sleep_time_min =  1; break;
                }
            break;
            case '1': est_config->sensors1 ^= AQDATA_CONTROL1_BAT_VALUE; break;
            case '2': est_config->sensors1 ^= AQDATA_CONTROL1_BME280;    break;
            case '3': est_config->sensors1 ^= AQDATA_CONTROL1_GPS;       break;
            case '4': est_config->sensors1 ^= AQDATA_CONTROL1_LUX;       break;
            case '5': est_config->sensors1 ^= AQDATA_CONTROL1_UV;        break;
            case '6': est_config->sensors1 ^= AQDATA_CONTROL1_WIND;      break;
            case '7': est_config->sensors1 ^= AQDATA_CONTROL1_RAIN;      break;
            case '8': est_config->sensors1 ^= AQDATA_CONTROL1_TBD;       break;

            case 'C': flash_clear_all(); break;
            case 'R': est_config_default(est_config); break;
            case 'W': flash_conf_write(est_config); break;
            case 'X': fim = true; break;
        }
    }   
}
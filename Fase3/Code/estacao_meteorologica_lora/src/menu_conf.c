#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "../include/aq_data.h"
#include "../include/code_config.h"
#include "../include/est_config.h"
#include "../include/flash.h"
#include "../include/menu_conf.h"

static void print_hex_array(uint8_t * array, int array_size){
    for(int i=0;i<array_size;i++){
        printf("%02x ", array[i]);
    }
}

static uint8_t char_to_uint8(char c){
    if((c >= '0') && (c <= '9')) return c - '0';
    if((c >= 'A') && (c <= 'F')) return c - 'A' + 10;
    if((c >= 'a') && (c <= 'f')) return c - 'a' + 10;
    return 0xFF;
}

static uint8_t inp_v[32];
static void input_hex(uint8_t * array, int array_size){
    int pos = 0;
    while(1){
        char c = getchar();
        if(c == '\r') return;
        if(c == '\n') return;
        if(c == 0x7F) { // back space
            if(pos){
                printf("\b");
                pos--;
            }
        }else{
            uint8_t v = char_to_uint8(c);
            if(v != 0xFF){
                inp_v[pos] = v;
                pos++;
                if(pos >= (array_size * 2)){//leu tudo
                    for(int i=0;i<array_size;i++){
                        array[i] = (inp_v[i*2] << 4) | inp_v[i*2+1];
                    }
                    return;
                }else{
                    printf("%c", c);
                }
            }
        }
    }
}

void menu_conf(EstConfig * est_config){
    bool fim = false;
    while(!fim){
        printf("\033[2J\033[H");    // Faz com que o terminal volte para o início da tela

        printf("********** %s **********\n\n", NAME);
        printf("Version: %s - %s - Build: %s\n\n\n", VERSION, VERSION_DATA, BUILD);

        printf("M) LoRa Mode: ");
        switch(est_config->lora_mode){
            case LORA_MODE_LORA:
                printf("LoRa\n");
                printf("  H) cHannel        : %02d\n", est_config->lora_par.channel);
                printf("  F) sF             : %02d\n", est_config->lora_par.sf);
                printf("  D) Device Address : ");
                print_hex_array(est_config->lora_par.device_address, 4);
                printf("\n\n\n\n");
                break;
            case LORA_MODE_LORAWAN_ABP:
                printf("LoRaWAN ABP\n");
                if(est_config->lorawan_abp_par.channel == 0xFF) printf("  H) cHannel        : AUTO\n");
                                                           else printf("  H) cHannel        : %02d\n",   est_config->lorawan_abp_par.channel);
                if(est_config->lorawan_abp_par.sf      == 0xFF) printf("  F) sF             : AUTO\n");
                                                           else printf("  F) sF             : %02d\n",   est_config->lorawan_abp_par.sf);
                printf("  T) fcnT           : 0x%04x\n", est_config->lorawan_abp_par.fcnt);
                printf("  D) Device Address : ");
                print_hex_array(est_config->lorawan_abp_par.device_address, 4);
                printf("\n");
                printf("  A) App_s_key      : ");
                print_hex_array(est_config->lorawan_abp_par.app_s_key, 16);
                printf("\n");
                printf("  N) Nwk_s_key      : ");
                print_hex_array(est_config->lorawan_abp_par.nwk_s_key, 16);
                printf("\n");
                break;
            case LORA_MODE_LORAWAN_OTAA:
                printf("LoRaWAN OTAA\n");
                if(est_config->lorawan_otaa_par.channel == 0xFF) printf("  H) cHannel : AUTO\n");
                                                            else printf("  H) cHannel : %02d\n", est_config->lorawan_otaa_par.channel);
                if(est_config->lorawan_otaa_par.sf      == 0xFF) printf("  F) sF      : AUTO\n");
                                                            else printf("  F) sF      : %02d\n", est_config->lorawan_otaa_par.sf);
                printf("  D) Dev EUI : ");
                print_hex_array(est_config->lorawan_otaa_par.dev_eui, 8);
                printf("\n");
                printf("  A) App EUI : ");
                print_hex_array(est_config->lorawan_otaa_par.app_eui, 8);
                printf("\n");
                printf("  K) App Key : ");
                print_hex_array(est_config->lorawan_otaa_par.app_key, 16);
                printf("\n\n");
                break;
        }
        printf("\n\n");

        if(est_config->sleep_time_min) printf("S) Sleep time: %d minutes\n\n", est_config->sleep_time_min);
                                  else printf("S) Sleep time: 10 seconds (To Test)\n\n");

        printf("1) Battery level: %s\n", est_config->active_sensors.battery?"On":"Off");
        printf("2) BME280       : %s\n", est_config->active_sensors.bme280? "On":"Off");
        printf("3) GPS          : %s\n", est_config->active_sensors.gps?    "On":"Off");
        printf("4) Luximeter    : %s\n", est_config->active_sensors.lux?    "On":"Off");
        printf("5) UV Index     : NOT implemented\n");
        printf("6) Wind         : NOT implemented\n");
        printf("7) Rain         : NOT implemented\n");
        printf("8) TBD          : NOT implemented\n\n\n");

        if(est_config->leds_on) printf("L) LEDs state   : ON (not recommended)\n");
                           else printf("L) LEDs state   : OFF\n");
        switch(est_config->usb_mode){
            case USB_MODE_OFF   : printf("U) USB : OFF\n\n\n");           break;
            case USB_MODE_ON    : printf("U) USB : ON (not recommended)\n\n\n");            break;
            case USB_MODE_OFF_ON: printf("U) USB : OFF-ON Auto (not recommended)\n\n\n"); break;
        }

        printf("C) Clear all data\n");
        printf("R) Reset Configuration\n\n");
        printf("W) Write configuration\n\n");
        printf("X) eXit\n\n");

        switch(toupper(getchar())){
            case 'A':
                switch(est_config->lora_mode){
                    case LORA_MODE_LORA: break;
                    case LORA_MODE_LORAWAN_ABP:
                        printf("LoRaWAN ABP App_s_key : ");
                        input_hex(est_config->lorawan_abp_par.app_s_key, 16);
                        break;
                    case LORA_MODE_LORAWAN_OTAA:
                        printf("LoRaWAN OTAA App EUI : ");
                        input_hex(est_config->lorawan_otaa_par.app_eui, 8);
                        break;
                }
                break;
            case 'D':
                switch(est_config->lora_mode){
                    case LORA_MODE_LORA:
                        printf("LoRa Device Address : ");
                        input_hex(est_config->lora_par.device_address, 4);
                        break;
                    case LORA_MODE_LORAWAN_ABP:
                        printf("LoRaWAN ABP Device Address : ");
                        input_hex(est_config->lorawan_abp_par.device_address, 4);
                        break;
                    case LORA_MODE_LORAWAN_OTAA:
                        printf("LoRaWAN OTAA Dev EUI : ");
                        input_hex(est_config->lorawan_otaa_par.dev_eui, 8);
                        break;
                }
                break;
            case 'F':   // sf
                switch(est_config->lora_mode){
                    case LORA_MODE_LORA:
                        if(++est_config->lora_par.sf > 12) est_config->lora_par.sf = 7;
                        break;
                    case LORA_MODE_LORAWAN_ABP:
                        if(est_config->lorawan_abp_par.sf > 12) est_config->lorawan_abp_par.sf = 7;
                        else if(++est_config->lorawan_abp_par.sf > 12) est_config->lorawan_abp_par.sf = 0xFF;
                        break;
                    case LORA_MODE_LORAWAN_OTAA:
                        if(est_config->lorawan_otaa_par.sf > 12) est_config->lorawan_otaa_par.sf = 0x7;
                        else if(++est_config->lorawan_otaa_par.sf > 12) est_config->lorawan_otaa_par.sf = 0xFF;
                        break;
                }
                break;                
            case 'H':   // Channel
                switch(est_config->lora_mode){
                    case LORA_MODE_LORA:
                        if(++est_config->lora_par.channel > 63) est_config->lora_par.channel = 0;
                        break;
                    case LORA_MODE_LORAWAN_ABP:
                        if(++est_config->lorawan_abp_par.channel > 63) est_config->lorawan_abp_par.channel = 0xFF;
                        break;
                    case LORA_MODE_LORAWAN_OTAA:
                        if(++est_config->lorawan_otaa_par.channel > 63) est_config->lorawan_otaa_par.channel = 0xFF;
                        break;
                }
                break;
            case 'K':
                switch(est_config->lora_mode){
                    case LORA_MODE_LORA: break;
                    case LORA_MODE_LORAWAN_ABP: break;
                    case LORA_MODE_LORAWAN_OTAA:
                        printf("LoRaWAN OTAA App Key : ");
                        input_hex(est_config->lorawan_otaa_par.app_key, 16);
                        break;
                }
                break;
            case 'L':
                est_config->leds_on = !est_config->leds_on;
                break;
            case 'M':
                if(++est_config->lora_mode >= LORA_MODE_COUNT) est_config->lora_mode = 0;
                break;
            case 'N':
                switch(est_config->lora_mode){
                    case LORA_MODE_LORA: break;
                    case LORA_MODE_LORAWAN_ABP:
                        printf("LoRaWAN ABP nwk_s_key : ");
                        input_hex(est_config->lorawan_abp_par.nwk_s_key, 16);
                        break;
                    case LORA_MODE_LORAWAN_OTAA: break;
                }
                break;
            case 'S': 
                switch(est_config->sleep_time_min){
                    case  0: est_config->sleep_time_min =  1; break;
                    case  1: est_config->sleep_time_min =  2; break;
                    case  2: est_config->sleep_time_min =  5; break;
                    case  5: est_config->sleep_time_min = 10; break;
                    case 10: est_config->sleep_time_min = 15; break;
                    case 15: est_config->sleep_time_min = 20; break;
                    case 20: est_config->sleep_time_min = 30; break;
                    case 30: est_config->sleep_time_min = 45; break;
                    case 45: est_config->sleep_time_min = 60; break;
                    case 60: est_config->sleep_time_min =  0; break;
                    default: est_config->sleep_time_min =  0; break;
                }
                break;
            case 'T':
                switch(est_config->lora_mode){
                    case LORA_MODE_LORA: break;
                    case LORA_MODE_LORAWAN_ABP:
                        printf("LoRaWAN fcnt : ");
                        uint8_t fcnt[4];
                        fcnt[0] = (uint8_t)(est_config->lorawan_abp_par.fcnt >> 8);
                        fcnt[1] = (uint8_t)(est_config->lorawan_abp_par.fcnt & 0X00FF);
                        input_hex(fcnt, 2);
                        est_config->lorawan_abp_par.fcnt = ((uint16_t)fcnt[0] << 8) | fcnt[1];
                        break;
                    case LORA_MODE_LORAWAN_OTAA: break;
                }
                break;

            case 'U':
                if(++est_config->usb_mode >= USB_MODE_COUNT) est_config->usb_mode = 0;
                break;
                
            case '1': est_config->active_sensors.battery = !est_config->active_sensors.battery; break;
            case '2': est_config->active_sensors.bme280  = !est_config->active_sensors.bme280;  break;
            case '3': est_config->active_sensors.gps     = !est_config->active_sensors.gps;     break;
            case '4': est_config->active_sensors.lux     = !est_config->active_sensors.lux;     break;
            case '5':                                                                           break;
            case '6':                                                                           break;
            case '7':                                                                           break;
            case '8':                                                                           break;

            case 'C': flash_clear_all(); break;
            case 'R': est_config_default(est_config); break;
            case 'W': flash_conf_write(est_config); break;
            case 'X': fim = true; break;
        }
    }   
}
/**
 * @file    menu_config.c
 * @author  Carlos e Ricardo
 * @brief   Ativa o Menu para a configuração do sistema
 * - O Menu vai funcionar através de um terminal serial via USB
 * @version 0.1
 * @date    2025-09-05
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"

#include "../include/config.h"
#include "../include/config_data.h"
#include "../include/menu_config.h"
#include "../include/storage.h"

/**
 * @brief Retorna o String correspondente ON ou OFF
 * 
 * @param on 
 */
static void on_off(bool on){
    if(on) printf("ON\n");
      else printf("OFF\n");
}

/**
 * @brief Faz o prinf da porta
 * 
 * @param port 
 */
static void ad_port(uint8_t port){
    if(port > AD_MAX_PORT) printf("OFF\n");
    else{
        printf("Port %d\n", port);
    }
}

/**
 * @brief retorna o valor da próxima porta
 * 
 * @param port, porta atual
 * @return uint8_t, próxima porta
 */
static uint8_t ad_next(uint8_t port){
    if(port > AD_MAX_PORT) return 0;
    else return port + 1;
}

/**
 * @brief Lê um texto com tamanho máximo definido
 * - só aceita ASCII
 * 
 * @param msg, Endereço do string
 * @param size, tamanho máximo do string 
 */
static void read_string(char * msg, int16_t size){
    bool fim = false;
    int  pos = 0;

    while(!fim){
        char c = getchar();
        if((c =='\n')||(c =='\r')) fim = true;
        else if((c >= 0x20)&&(c <= 0x7E)){      // só aceita ASCII
            msg[pos] = c;
            printf("%c", c);
            pos++;
            if(pos >= (size -1)) fim = true;
        }
    }
    msg[pos] = 0;
}

/**
 * @brief Ativa o Menu para a configuração do sistema
 * - O Menu vai funcionar através de um terminal serial via USB
 * 
 */
void menu_config_init(){
    bool fim = false;
    while(!fim){
        printf("\033[2J\033[H");
        printf("***** Configuração da estação Meteorológica *****\n\n");
        printf("Code Build: %s Config Version: %d\n\n", SW_BUILD, CONFIG_DATA_VERSION);

        printf("-- Parâmetros da Estação --\n");
        printf("1) Nome      : %s\n",          config_data.station_name);
        printf("2) Token(PSW): %s\n",          config_data.station_token);
        printf("3) Grupo     : %s\n",          config_data.station_group);
        printf("4) Latitude  : %s graus\n",    config_data.station_latitude);
        printf("5) Longitude : %s graus\n",    config_data.station_longitude);
        printf("6) Elevação  : %s metros\n\n", config_data.station_elevation);

        printf("-- Parâmetros da Comunicação --\n");
        printf("7) WiFi SSID        : %s\n",   config_data.wifi_ssid);
        //printf("8) WiFi PSW       : %s\n",   config_data.wifi_psw);
        printf("8) WiFi PSW         : ********\n");
        printf("9) IP do Servidor   : %s\n",   config_data.server_ip);
        printf("0) Porta do Servidor: %u\n\n", config_data.server_port);
// KLMP
        printf("-- Definição dos intervalos e clock --\n");
        printf("A) Intervalo de captura  : %s\n",   config_data_period_string(config_data.period_acquisition));
        printf("B) Intervalo de envio    : %s\n",   config_data_period_string(config_data.period_send));
        printf("C) Intervalo de show     : %s\n",   config_data_period_string(config_data.period_show));
        printf("D) Intervalo de sync CLK : %s\n",   config_data_period_string(config_data.period_sync_clk));
        printf("E) Intervalo de retry    : %s\n",   config_data_period_string(config_data.period_retry));
        printf("F) RTC enable            : ");      on_off(config_data.rtc_with);    

        printf("\n-- Sensores I2C ativos --\n");
        printf("G) Pressão e Temperatura (BMP280) : "); on_off(config_data.sensor_i2c_pressure);
        printf("H) Umidade e Temperatura (AHT10)  : "); on_off(config_data.sensor_i2c_humidity);
        printf("I) Intensidade Luminosa  (BH1750) : "); on_off(config_data.sensor_i2c_luminosity);
        printf("J) Direção do Vento      (HMC5883): "); on_off(config_data.sensor_i2c_wind_direction);
        // K
        // L
        printf("\n-- Sensores digitais ativos --\n");
        printf("M) Anemômetro  : "); on_off(config_data.sensor_digital_wind_speed);
        printf("N) Pluviômetro : "); on_off(config_data.sensor_digital_rain_intensity);
        // O
        // P
        printf("\n-- Sensores analógicos ativos --\n");
        printf("Q) Índice UV           : "); ad_port(config_data.sensor_analog_uv_index);
        printf("R) Temperatura do solo : "); ad_port(config_data.sensor_analog_soil_temperature);
        printf("S) Umidade do solo     : "); ad_port(config_data.sensor_analog_soil_humidity);
        printf("T) Concentração CO     : "); ad_port(config_data.sensor_analog_ar_co);
        printf("U) Concentração CO2    : "); ad_port(config_data.sensor_analog_ar_co2);
        printf("V) Concentração O3     : "); ad_port(config_data.sensor_analog_ar_o3);
        
        printf("\nY) Reset Configuração\n");
        printf("Z) Limpa todos os dados\n");
        
        printf("\nW) Grava os Dados (Write)\n");
        printf("\nX) Sair (eXit)\n\n");

        char tmp[16] = {0};
        double aux_d;
        switch(toupper(getchar())){
            // -- Parãmetros da Estação --
            case '1':  printf("Nome da estação : ");  read_string(config_data.station_name,      sizeof(config_data.station_name));      break;
            case '2':  printf("Token da estação : "); read_string(config_data.station_token,     sizeof(config_data.station_token));     break;
            case '3':  printf("Grupo da estação : "); read_string(config_data.station_group,     sizeof(config_data.station_group));     break;
            case '4':
                printf("Latitude : ");
                read_string(tmp, 15);
                if(sscanf(tmp, "%lf", &aux_d) == 1){
                    if((aux_d <= 90.0)&&(aux_d >= -90.0)){
                       sprintf(config_data.station_latitude, "%.7lf", aux_d); 
                    }
                }
                break;
            case '5':
                printf("Longitude : ");
                read_string(tmp, 15);
                if(sscanf(tmp, "%lf", &aux_d) == 1){
                    if((aux_d <= 180.0)&&(aux_d >= -180.0)){
                       sprintf(config_data.station_longitude, "%.7lf", aux_d); 
                    }
                }
                break;
            case '6':  
                printf("Elevação : ");
                read_string(tmp, 15);
                if(sscanf(tmp, "%lf", &aux_d) == 1){
                    if((aux_d <= 10000.0)&&(aux_d >= -1000.0)){
                       sprintf(config_data.station_elevation, "%.1lf", aux_d); 
                    }
                }
                break;

            // -- Parâmetros da Comunicação -
            case '7': printf("WiFi SSID : ");         read_string(config_data.wifi_ssid,   sizeof(config_data.wifi_ssid));   break;
            case '8': printf("WiFi PSW : ");          read_string(config_data.wifi_psw,    sizeof(config_data.wifi_psw));    break;
            case '9': printf("IP do Servidor : ");    read_string(config_data.server_ip,  sizeof(config_data.server_ip));  break;
            case '0': 
                printf("Porta do Servidor : ");
                read_string(tmp, 15);
                uint32_t aux;
                if(sscanf(tmp, "%u", &aux) == 1){
                    if(aux <= 0xFFFF){
                        config_data.server_port = aux;
                    }
                }
                break;

            // -- Definição dos intervalos --
            case 'A': 
                if(config_data.period_acquisition >= PERIOD_ACQUISITION_MAX) config_data.period_acquisition = PERIOD_ACQUISITION_MIN;
                else config_data.period_acquisition++;
                break;
            case 'B': 
                //if(config_data.period_send >= PERIOD_SEND_MAX) config_data.period_send = PERIOD_SEND_MIN;
                if(config_data.period_send >= PERIOD_N) config_data.period_send = PERIOD_SEND_MIN;
                else config_data.period_send++;
                break;
            case 'C': 
                //if(config_data.period_show >= PERIOD_1DAY) config_data.period_show = PERIOD_1SEC;
                if(config_data.period_show >= PERIOD_N) config_data.period_show = PERIOD_1SEC;
                else config_data.period_show++;
                break;
            case 'D': 
                if(config_data.period_sync_clk >= PERIOD_N) config_data.period_sync_clk = PERIOD_1SEC;
                else config_data.period_sync_clk++;
                break;
            case 'E': 
                if(config_data.period_retry >= PERIOD_SEND_MAX) config_data.period_retry = 0;
                else config_data.period_retry++;
                break;

            case 'F': config_data.rtc_with                  = !config_data.rtc_with;                  break;
            // -- Sensores I2C ativos --
            case 'G': config_data.sensor_i2c_pressure       = !config_data.sensor_i2c_pressure;       break;
            case 'H': config_data.sensor_i2c_humidity       = !config_data.sensor_i2c_humidity;       break;
            case 'I': config_data.sensor_i2c_luminosity     = !config_data.sensor_i2c_luminosity;     break;
            case 'J': config_data.sensor_i2c_wind_direction = !config_data.sensor_i2c_wind_direction; break;

            // -- Sensores digitais ativos --
            case 'M': config_data.sensor_digital_wind_speed     = !config_data.sensor_digital_wind_speed;     break;
            case 'N': config_data.sensor_digital_rain_intensity = !config_data.sensor_digital_rain_intensity; break;

            // -- Sensores analógicos ativos --
            case 'Q':
                config_data.sensor_analog_uv_index         = ad_next(config_data.sensor_analog_uv_index);
                break;
            case 'R':
                config_data.sensor_analog_soil_temperature = ad_next(config_data.sensor_analog_soil_temperature);
                break;
            case 'S':
                config_data.sensor_analog_soil_humidity    = ad_next(config_data.sensor_analog_soil_humidity);
                break;
            case 'T':
                config_data.sensor_analog_ar_co            = ad_next(config_data.sensor_analog_ar_co);
                break;
            case 'U':
                config_data.sensor_analog_ar_co2           = ad_next(config_data.sensor_analog_ar_co2);
                break;
            case 'V':
                config_data.sensor_analog_ar_o3            = ad_next(config_data.sensor_analog_ar_o3);
                break;

            // -- Comandos Gerais --
            case 'Y':
                config_data_set_default();
                break;
            case 'Z':
                printf("Limpando os dados\n");
                stored_data_clear_all();
                break;
            case 'W':
                config_data_storage_write();
                //fim = true;
                break;
            case 'X':
                //config_data_storage_write();
                fim = true;
                break;
        }
    }
    printf("\n***** FIM da Configuração da estação Meteorológica *****\n\n");
}
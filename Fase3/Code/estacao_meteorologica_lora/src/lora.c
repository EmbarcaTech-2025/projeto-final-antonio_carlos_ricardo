#include <stdio.h>
#include "pico/stdlib.h"
#include "../include/aq_data.h"
#include "../include/lora.h"

void lora_send(EstConfig *est_config, AqData *aq_data){
    printf("----- Inicio da transmissão LoRa -----\n");
    if(aq_data->control1 & AQDATA_CONTROL1_BAT_VALUE){
        if(aq_data->battery.bat_level == 0xFF) printf("- ERROR Battery sensor\n");
        else{
            printf("- Battery Level = %.1f %%\n", aq_data->battery.bat_level * 0.5);
        }
    }else printf("- NO Battery sensor\n");

    if(aq_data->control1 & AQDATA_CONTROL1_BME280){
        if(aq_data->bme280.humidity == 0xFF) printf("- ERROR BME280 Humidity sensor\n");
        else{
            printf("- BMP280 Humidity    = %.1f %%\n", aq_data->bme280.humidity * 0.5);
        }
        if(aq_data->bme280.press == 0xFFFF) printf("- ERROR BME280 Pressure sensor\n");
        else{
            printf("- BMP280 pressure    = %.2f hPa\n", (60000 - aq_data->bme280.press) * 0.02);
        }
        if(aq_data->bme280.temp == 0x7FFF) printf("- ERROR BME280 Temperature sensor\n");
        else{
            printf("- BMP280 Temperature = %.2f Celsius\n", aq_data->bme280.temp * 0.01);
        }
    }else printf("- NO BME280 sensor\n");

    if(aq_data->control1 & AQDATA_CONTROL1_GPS){
        if(aq_data->gps.latitude == 0x7FFFFFFF) printf("- ERROR GPS latitude\n");
        else{
            printf("- GPS latitude  = %f degrees\n", aq_data->gps.latitude / 8388608.0);
        }
        if(aq_data->gps.longitude == 0x7FFFFFFF) printf("- ERROR GPS longitude\n");
        else{
            printf("- GPS longitude = %f degrees\n", aq_data->gps.longitude / 8388608.0);
        }

        if(aq_data->gps.altitude == 0x7FFF) printf("- ERROR GPS altitude\n");
        else{
            printf("- GPS altitude  = %.1f meters\n", aq_data->gps.altitude * 0.1);
        }
    }else printf("- NO GPS\n");

    printf("----- Fim da transmissão LoRa -----\n\n");
    
}
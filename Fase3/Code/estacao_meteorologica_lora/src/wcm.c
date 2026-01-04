#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "../include/aq_data.h"
#include "../include/code_config.h"
#include "../include/wcm.h"

static uint8_t payload[128];
static char    msg[256];





static void add_hex(uint8_t * hex, int hex_size){
    int p = strlen(msg);
    msg[p++] = ' ';
    msg[p] = 0;
    for(int i=0;i<hex_size;i++){
        sprintf(&msg[p], "%02X", hex[i]);
        p += 2;
    }
}

void wcm_init(){
    uart_init(WCM_UART_ID, WCM_UART_BAUD_RATE);
    gpio_set_function(WCM_UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(WCM_UART_RX_PIN, GPIO_FUNC_UART);
}

static int count = 0;
void wcm_send(EstConfig *est_config, AqData *aq_data){
    // Inicializa UART
    uart_init(WCM_UART_ID, WCM_UART_BAUD_RATE);
    gpio_set_function(WCM_UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(WCM_UART_RX_PIN, GPIO_FUNC_UART);
    sleep_ms(10);





    /*
    
    uart_putc_raw(WCM_UART_ID, ' ');
    sleep_ms(100);
    uart_write_blocking(WCM_UART_ID, ex, strlen(ex));
    printf("Mandei\n");
    sleep_ms(5000);
    return;*/

#ifdef SEND_FIX_DATA
    if(count < 5){
        aq_data->battery.bat_level =        153;    // 76.5 %
        aq_data->bme280.humidity   =         69;    // 34.5 %
        aq_data->bme280.temp       =       2543;    // 25.43 Celsius
        aq_data->bme280.press      =       9338;    // 1013.24 hPa
        aq_data->gps.latitude      = -191405737;    // -22,817341924
        aq_data->gps.longitude     = -394864185;    // -47,071478963
        aq_data->gps.altitude      =       4321;    // 432.1 m

        aq_data->battery.bat_level += count;
        aq_data->bme280.humidity   += count;
        aq_data->bme280.temp       += count;
        aq_data->bme280.press      += count;
        aq_data->gps.latitude      += count;
        aq_data->gps.longitude     += count;
        aq_data->gps.altitude      += count;

        count++;
    }
#endif


    int payload_pos = 0;
    payload[payload_pos++] = aq_data->control1;

    printf("----- Inicio da transmissão LoRa -----\n");
    if(aq_data->control1 & AQDATA_CONTROL1_BAT_VALUE){
        if(aq_data->battery.bat_level == 0xFF) printf("- ERROR Battery sensor\n");
        else{
            printf("- Battery Level = %.1f %%\n", aq_data->battery.bat_level * 0.5);
        }
        payload[payload_pos++] = aq_data->battery.bat_level;
    }else printf("- NO Battery sensor\n");

    if(aq_data->control1 & AQDATA_CONTROL1_BME280){
        if(aq_data->bme280.humidity == 0xFF) printf("- ERROR BME280 Humidity sensor\n");
        else{
            printf("- BMP280 Humidity    = %.1f %%\n", aq_data->bme280.humidity * 0.5);
        }
        payload[payload_pos++] = aq_data->bme280.humidity;


        if(aq_data->bme280.temp == 0x7FFF) printf("- ERROR BME280 Temperature sensor\n");
        else{
            printf("- BMP280 Temperature = %.2f Celsius\n", aq_data->bme280.temp * 0.01);
        }
        payload[payload_pos++] = aq_data->bme280.temp >> 8; 
        payload[payload_pos++] = aq_data->bme280.temp & 0x00FF;


        if(aq_data->bme280.press == 0xFFFF) printf("- ERROR BME280 Pressure sensor\n");
        else{
            printf("- BMP280 pressure    = %.2f hPa\n", (60000 - aq_data->bme280.press) * 0.02);
        }
        payload[payload_pos++] = aq_data->bme280.press >> 8; 
        payload[payload_pos++] = aq_data->bme280.press & 0x00FF;
    }else printf("- NO BME280 sensor\n");

    if(aq_data->control1 & AQDATA_CONTROL1_GPS){
        if(aq_data->gps.latitude == 0x7FFFFFFF) printf("- ERROR GPS latitude\n");
        else{
            printf("- GPS latitude  = %f degrees\n", aq_data->gps.latitude / 8388608.0);
        }
        payload[payload_pos++] =  aq_data->gps.latitude >> 24;
        payload[payload_pos++] = (aq_data->gps.latitude >> 16) & 0x000000FF;  
        payload[payload_pos++] = (aq_data->gps.latitude >>  8) & 0x000000FF;  
        payload[payload_pos++] =  aq_data->gps.latitude        & 0x000000FF;


        if(aq_data->gps.longitude == 0x7FFFFFFF) printf("- ERROR GPS longitude\n");
        else{
            printf("- GPS longitude = %f degrees\n", aq_data->gps.longitude / 8388608.0);
        }
        payload[payload_pos++] =  aq_data->gps.longitude >> 24;
        payload[payload_pos++] = (aq_data->gps.longitude >> 16) & 0x000000FF;  
        payload[payload_pos++] = (aq_data->gps.longitude >>  8) & 0x000000FF;  
        payload[payload_pos++] =  aq_data->gps.longitude        & 0x000000FF;
        

        if(aq_data->gps.altitude == 0x7FFF) printf("- ERROR GPS altitude\n");
        else{
            printf("- GPS altitude  = %.1f meters\n", aq_data->gps.altitude * 0.1);
        }
        payload[payload_pos++] = aq_data->gps.altitude >> 8; 
        payload[payload_pos++] = aq_data->gps.altitude & 0x00FF;
    }else printf("- NO GPS\n");
    printf("\nPayload len=%d: 0x", payload_pos);
    for(int i=0;i<payload_pos;i++)
    printf(" %02x", payload[i]);
    printf("\n");

    
    sprintf(msg, "A %02d %02d %08X %04X",
        est_config->lora_abp_par.channel,
        est_config->lora_abp_par.sf,
        est_config->lora_abp_par.device_address,
        est_config->lora_abp_par.fcnt++);
    add_hex(est_config->lora_abp_par.app_s_key, 16);
    add_hex(est_config->lora_abp_par.net_s_key, 16);
    add_hex(payload, payload_pos);
    printf("CMD: %s\n", msg);

    uart_putc_raw(WCM_UART_ID, ' ');
    sleep_ms(100);
    
    while(uart_is_readable(WCM_UART_ID)){
        uart_getc(WCM_UART_ID);
        sleep_us(100);
    }
    
    uart_write_blocking(WCM_UART_ID, msg, strlen(msg));
    uart_putc_raw(WCM_UART_ID, '\n');
    printf("Enviou comando\n");


    while(!uart_is_readable(WCM_UART_ID));
    printf("Teve resposta\n");

    sleep_ms(5000);
    


    printf("----- Fim da transmissão LoRa -----\n\n");
    
}
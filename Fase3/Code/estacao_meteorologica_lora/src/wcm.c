#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "../include/aq_data.h"
#include "../include/code_config.h"
#include "../include/loop_printf.h"
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
    loop_printf("Mandei\n");
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

    if(est_config->lora_mode == LORA_MODE_LORA){
        payload[payload_pos++] = est_config->lora_par.device_address[3];
        payload[payload_pos++] = est_config->lora_par.device_address[2];
        payload[payload_pos++] = est_config->lora_par.device_address[1];
        payload[payload_pos++] = est_config->lora_par.device_address[0];
    }

    uint8_t control1 = 0;
    if(aq_data->active_sensors.battery) control1 |= AQ_ITEM_BAT_VALUE;
    if(aq_data->active_sensors.bme280)  control1 |= AQ_ITEM_BME280;
    if(aq_data->active_sensors.gps)     control1 |= AQ_ITEM_GPS;
    if(aq_data->active_sensors.lux)     control1 |= AQ_ITEM_LUX;
    if(aq_data->active_sensors.uv)      control1 |= AQ_ITEM_UV;
    if(aq_data->active_sensors.wind)    control1 |= AQ_ITEM_WIND;
    if(aq_data->active_sensors.rain)    control1 |= AQ_ITEM_RAIN;
    if(aq_data->active_sensors.tbd)     control1 |= AQ_ITEM_TBD;
    payload[payload_pos++] = control1;

    loop_printf("----- Inicio da transmissão LoRa -----\n");
    if(aq_data->active_sensors.battery){
        if(aq_data->battery.bat_level == 0xFF) loop_printf("- ERROR Battery sensor\n");
        else{
            loop_printf("- Battery Level = %.1f %%\n", aq_data->battery.bat_level * 0.5);
        }
        payload[payload_pos++] = aq_data->battery.bat_level;
    }else loop_printf("- NO Battery sensor\n");

    if(aq_data->active_sensors.bme280){
        if(aq_data->bme280.humidity == 0xFF) loop_printf("- ERROR BME280 Humidity sensor\n");
        else{
            loop_printf("- BME280 Humidity    = %.1f %%\n", aq_data->bme280.humidity * 0.5);
        }
        payload[payload_pos++] = aq_data->bme280.humidity;


        if(aq_data->bme280.temp == 0x7FFF) loop_printf("- ERROR BME280 Temperature sensor\n");
        else{
            loop_printf("- BME280 Temperature = %.2f Celsius\n", aq_data->bme280.temp * 0.01);
        }
        payload[payload_pos++] = aq_data->bme280.temp >> 8; 
        payload[payload_pos++] = aq_data->bme280.temp & 0x00FF;


        if(aq_data->bme280.press == 0xFFFF) loop_printf("- ERROR BME280 Pressure sensor\n");
        else{
            loop_printf("- BME280 pressure    = %.2f hPa\n", (60000 - aq_data->bme280.press) * 0.02);
        }
        payload[payload_pos++] = aq_data->bme280.press >> 8; 
        payload[payload_pos++] = aq_data->bme280.press & 0x00FF;
    }else loop_printf("- NO BME280 sensor\n");

    if(aq_data->active_sensors.gps){
        if(aq_data->gps.latitude == 0x7FFFFFFF) loop_printf("- ERROR GPS latitude\n");
        else{
            loop_printf("- GPS latitude  = %f degrees\n", aq_data->gps.latitude / 8388608.0);
        }
        payload[payload_pos++] =  aq_data->gps.latitude >> 24;
        payload[payload_pos++] = (aq_data->gps.latitude >> 16) & 0x000000FF;  
        payload[payload_pos++] = (aq_data->gps.latitude >>  8) & 0x000000FF;  
        payload[payload_pos++] =  aq_data->gps.latitude        & 0x000000FF;


        if(aq_data->gps.longitude == 0x7FFFFFFF) loop_printf("- ERROR GPS longitude\n");
        else{
            loop_printf("- GPS longitude = %f degrees\n", aq_data->gps.longitude / 8388608.0);
        }
        payload[payload_pos++] =  aq_data->gps.longitude >> 24;
        payload[payload_pos++] = (aq_data->gps.longitude >> 16) & 0x000000FF;  
        payload[payload_pos++] = (aq_data->gps.longitude >>  8) & 0x000000FF;  
        payload[payload_pos++] =  aq_data->gps.longitude        & 0x000000FF;
        

        if(aq_data->gps.altitude == 0x7FFF) loop_printf("- ERROR GPS altitude\n");
        else{
            loop_printf("- GPS altitude  = %.1f meters\n", aq_data->gps.altitude * 0.1);
        }
        payload[payload_pos++] = aq_data->gps.altitude >> 8; 
        payload[payload_pos++] = aq_data->gps.altitude & 0x00FF;
    }else loop_printf("- NO GPS\n");
    loop_printf("\nPayload len=%d: 0x", payload_pos);
    for(int i=0;i<payload_pos;i++)
    loop_printf(" %02x", payload[i]);
    loop_printf("\n");


    // falta colocar os outros

    
    sprintf(msg, "A %02d %02d %02X%02X%02X%02X %04X",
        est_config->lorawan_abp_par.channel,
        est_config->lorawan_abp_par.sf,
        est_config->lorawan_abp_par.device_address[3],
        est_config->lorawan_abp_par.device_address[2],
        est_config->lorawan_abp_par.device_address[1],
        est_config->lorawan_abp_par.device_address[0],
        est_config->lorawan_abp_par.fcnt++);
    add_hex(est_config->lorawan_abp_par.app_s_key, 16);
    add_hex(est_config->lorawan_abp_par.nwk_s_key, 16);
    add_hex(payload, payload_pos);
    loop_printf("CMD: %s\n", msg);

    uart_putc_raw(WCM_UART_ID, ' ');
    sleep_ms(100);
    
    while(uart_is_readable(WCM_UART_ID)){
        uart_getc(WCM_UART_ID);
        sleep_us(100);
    }
    
    uart_write_blocking(WCM_UART_ID, msg, strlen(msg));
    uart_putc_raw(WCM_UART_ID, '\n');
    loop_printf("Enviou comando\n");


    while(!uart_is_readable(WCM_UART_ID));
    loop_printf("Teve resposta\n");

    //sleep_ms(5000);
    


    loop_printf("----- Fim da transmissão LoRa -----\n\n");
    
}
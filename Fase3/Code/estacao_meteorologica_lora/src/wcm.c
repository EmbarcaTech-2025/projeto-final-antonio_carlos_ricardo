#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "../include/aq_data.h"
#include "../include/code_config.h"
#include "../include/loop_printf.h"
#include "../include/wcm.h"
#include "../include/wrap_watchdog.h"

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

#ifdef SEND_FIX_DATA
    if(count < 5){
        aq_data->battery.bat_level =        153;    // 76.5 %
        aq_data->bmep280.humidity  =         69;    // 34.5 %
        aq_data->bmep280.temp      =       2543;    // 25.43 Celsius
        aq_data->bmep280.press     =       9338;    // 1013.24 hPa
        aq_data->gps.latitude      = -191405737;    // -22,817341924
        aq_data->gps.longitude     = -394864185;    // -47,071478963
        aq_data->gps.altitude      =       4321;    // 432.1 m
        aq_data->lux.lux_level_x4  =      12345;
        aq_data->vsys              =        100;    // 2000mv
        aq_data->cpu_temp_deci     =        -20;    // -2 C

        aq_data->battery.bat_level += count;
        aq_data->bmep280.humidity  += count;
        aq_data->bmep280.temp      += count;
        aq_data->bmep280.press     += count;
        aq_data->gps.latitude      += count;
        aq_data->gps.longitude     += count;
        aq_data->gps.altitude      += count;
        aq_data->lux.lux_level_x4  += count * 1000;
        aq_data->vsys              += count;
        aq_data->cpu_temp_deci     += count * 10;

        count++;
    }
#endif

    int payload_pos = 0;

    if(est_config->lora_mode == LORA_MODE_LORA){
        payload[payload_pos++] = est_config->lora_par.device_address[0];
        payload[payload_pos++] = est_config->lora_par.device_address[1];
        payload[payload_pos++] = est_config->lora_par.device_address[2];
        payload[payload_pos++] = est_config->lora_par.device_address[3];
    }

    uint8_t control1 = 0;
    if(aq_data->active_sensors.battery)  control1 |= AQ_ITEM_BAT_VALUE;
    if(aq_data->active_sensors.bme280)   control1 |= AQ_ITEM_BME280;
    if(aq_data->active_sensors.gps)      control1 |= AQ_ITEM_GPS;
    if(aq_data->active_sensors.lux)      control1 |= AQ_ITEM_LUX;
    if(aq_data->active_sensors.uv)       control1 |= AQ_ITEM_UV;
    if(aq_data->active_sensors.wind)     control1 |= AQ_ITEM_WIND;
    if(aq_data->active_sensors.vsys)     control1 |= AQ_ITEM_VSYS;
    if(aq_data->active_sensors.cpu_temp) control1 |= AQ_ITEM_CPU_TEMP;   //AQ_ITEM_TBD;
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
        if(aq_data->bmep280.humidity == 0xFF) loop_printf("- ERROR BME280 Humidity sensor\n");
        else{
            loop_printf("- BME280 Humidity    = %.1f %%\n", aq_data->bmep280.humidity * 0.5);
        }
        payload[payload_pos++] = aq_data->bmep280.humidity;


        if(aq_data->bmep280.temp == 0x7FFF) loop_printf("- ERROR BME280 Temperature sensor\n");
        else{
            loop_printf("- BME280 Temperature = %.2f Celsius\n", aq_data->bmep280.temp * 0.01);
        }
        payload[payload_pos++] = aq_data->bmep280.temp >> 8; 
        payload[payload_pos++] = aq_data->bmep280.temp & 0x00FF;


        if(aq_data->bmep280.press == 0xFFFF) loop_printf("- ERROR BME280 Pressure sensor\n");
        else{
            loop_printf("- BME280 pressure    = %.2f hPa\n", (60000 - aq_data->bmep280.press) * 0.02);
        }
        payload[payload_pos++] = aq_data->bmep280.press >> 8; 
        payload[payload_pos++] = aq_data->bmep280.press & 0x00FF;
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

    if(aq_data->active_sensors.lux){
        if(aq_data->lux.lux_level_x4 == 0xFFFF) loop_printf("- ERROR Lux Meter sensor\n");
        else{
            loop_printf("- Lux Meter = %u lux\n", aq_data->lux.lux_level_x4 * 4);
        }
        payload[payload_pos++] = aq_data->lux.lux_level_x4 >> 8; 
        payload[payload_pos++] = aq_data->lux.lux_level_x4 & 0x00FF;
    }else loop_printf("- NO Lux Meter sensor\n");


    if(aq_data->active_sensors.vsys){
        if(aq_data->vsys == 0xFF) loop_printf("- ERROR VSys sensor\n");
        else{
            loop_printf("- VSys     = %4d mV\n", aq_data->vsys * 20);
        }
        payload[payload_pos++] = aq_data->vsys;
    }else loop_printf("- NO VSys sensor\n");

    if(aq_data->active_sensors.cpu_temp){
        if(aq_data->cpu_temp_deci == 0x7F) loop_printf("- ERROR CPU Temp sensor\n");
        else{
            loop_printf("- CPU Temp = %.1f Celsius\n", ((aq_data->cpu_temp_deci / 5)) * 0.5);
        }
        //payload[payload_pos++] = (aq_data->cpu_temp_deci / 5);
        payload[payload_pos++] = (aq_data->cpu_temp_deci / 10);
    }else loop_printf("- NO CPU Temp sensor\n");

    //  Payload gerado
    loop_printf("\nPayload len=%d: 0x", payload_pos);
    for(int i=0;i<payload_pos;i++)
    loop_printf(" %02x", payload[i]);
    loop_printf("\n");

    switch(est_config->lora_mode){
        case LORA_MODE_LORA:
            // Example:  L ch sf -Message\\n\n"
            // Example1: L 05 07 12345678\n"
            sprintf(msg, "O %02x %02x",
                est_config->lorawan_otaa_par.channel,
                est_config->lorawan_otaa_par.sf);
            add_hex(payload, payload_pos);
            break;
        case LORA_MODE_LORAWAN_ABP:
            // Example  : A ch sf devaddr  fcnt ------------appskey------------- ------------nwkskey------------- -Message--\n
            // Example 1: A 05 07 12345678 4321 00112233445566778899AABBCCDDEEFF FFEEDDCCBBAA99887766554433221100 0987654321\n
            sprintf(msg, "A %02x %02x %02X%02X%02X%02X %04X",
                est_config->lorawan_abp_par.channel,
                est_config->lorawan_abp_par.sf,
                est_config->lorawan_abp_par.device_address[0],
                est_config->lorawan_abp_par.device_address[1],
                est_config->lorawan_abp_par.device_address[2],
                est_config->lorawan_abp_par.device_address[3],
                est_config->lorawan_abp_par.fcnt++);
            add_hex(est_config->lorawan_abp_par.app_s_key, 16);
            add_hex(est_config->lorawan_abp_par.nwk_s_key, 16);
            add_hex(payload, payload_pos);
            break;
        case LORA_MODE_LORAWAN_OTAA:
            // Example: O ch sf -----DeveUi----- -----AppUi------ -------------AppKey------------- -Message--\n"
            // Example: O 05 07 0011223344556677 8899AABBCCDDEEFF FFEEDDCCBBAA99887766554433221100 1234567890\n"
            sprintf(msg, "O %02x %02x",
                est_config->lorawan_otaa_par.channel,
                est_config->lorawan_otaa_par.sf);
                add_hex(est_config->lorawan_otaa_par.dev_eui,  8);
                add_hex(est_config->lorawan_otaa_par.app_eui,  8);
                add_hex(est_config->lorawan_otaa_par.app_key, 16);
                add_hex(payload, payload_pos);
            break;    
    }

    loop_printf("CMD: %s\n", msg);

    // Acordar WCM
    uart_putc_raw(WCM_UART_ID, ' ');
    sleep_ms(100);
    
    //limpar WCM
    while(uart_is_readable(WCM_UART_ID)){
        uart_getc(WCM_UART_ID);
        sleep_us(100);
    }
    
    uart_write_blocking(WCM_UART_ID, msg, strlen(msg));
    uart_putc_raw(WCM_UART_ID, '\n');
    loop_printf("Enviou comando\n");


    uint64_t t_timeout = time_us_64() + WCM_TIMEOUT_US;
    while((!uart_is_readable(WCM_UART_ID))&&(t_timeout > time_us_64())){
        wrap_watchdog_update();
    }
    if(uart_is_readable(WCM_UART_ID)){
        char ret_c = uart_getc(WCM_UART_ID);
        loop_printf("Teve resposta: %c\n", ret_c);
    }else{
        loop_printf("Time OUT\n");
    }
    

    loop_printf("----- Fim da transmissão LoRa -----\n\n");    
}
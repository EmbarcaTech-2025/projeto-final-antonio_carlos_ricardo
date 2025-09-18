/**
 * @file    main.c
 * @author  Carlos e Ricardo
 * @brief   Inicialização do sistema e main loop para transmissão de dados
 * @version 0.1
 * @date    2025-09-04
 * 
 * @copyright Copyright (c) 2025
 */

#include <stdio.h>
#include <pico/cyw43_arch.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "hardware/i2c.h"
#include "include/config.h"
#include "include/config_data.h"
#include "include/core_communication.h"
#include "include/core0_comm.h"
#include "include/core1_main.h"
#include "include/led_rgb.h"
#include "include/menu_config.h"
#include "include/menu_clock.h"
#include "include/storage.h"
#include "include/ui_display.h"
#include "include/util.h"
#include "include/wrap_watchdog.h"
#include "include/wifi.h"
#include "include/wifi_tcp.h"

#define PAGE_SEND       "/dados"
#define PAGE_TIME       "/hora"


typedef enum {
    WIFI_ST_DISCONNECTED = 0,
    WIFI_ST_CONNECTED_NO_ERROR,
    WIFI_ST_CONNECTED_WITH_ERROR
} WiFiState;



/**
 * @brief Envia uma aquisição completa, ainda não enviada, para o servidor
 * 
 * @return true, o servidor recebeu a mensagem
 * @return false, o servidor não recebeu a mensagem
 */
static bool send_data(){
    char * url = config_data.server_ip;
    ip_addr_t ip;
    ip4addr_aton(url, &ip);

    int16_t size = estation_data_json(&core_comm_estation_data, estation_data_buf_tmp);
    //static uint32_t send_data_count = 0;
    //int16_t size = estation_data_json_full_fake(estation_data_buf_tmp, ++send_data_count);
    WifiTcpData * ret = wifi_tcp_send(ip, url, config_data.server_port, PAGE_SEND, estation_data_buf_tmp);

    if(ret == NULL) return false;
               else return wifi_tcp_answer_send(ret); 
}

/**
 * @brief  Get the server time object
 * 
 * @return true, recebeu a data/hora atual do servidor e atualizou o relógio local
 * @return false, não conseguiu receber a data/hora atual do servidor
 */
static bool get_server_time(){
    char * url = config_data.server_ip;
    ip_addr_t ip;
    ip4addr_aton(url, &ip);

    char jsont_to_send[48] = {0};
    sprintf(jsont_to_send, "{\"id\":\"%s\",\"token\":\"%s\"}", config_data.station_name, config_data.station_token);

    WifiTcpData * ret = wifi_tcp_send(ip, url, config_data.server_port, PAGE_TIME, jsont_to_send);

    if(ret == NULL) return false;
    else{
        uint64_t new_time;
        if(wifi_tcp_answer_get_time(ret, &new_time)){
            core0_comm_new_time(new_time);
            return true;
        }else{
            return false;
        }
    }
}

/**
 * @brief Inicializa todo o sistema, ativa o core1 e mantem ativo um looping infinito para transmitir os dados
 * 
 */
int main()
{
    stdio_init_all();

#ifdef BITDOGLAB_WITH_DEBUG_PROBE
    gpio_set_function( 0, GPIO_FUNC_SIO);
    gpio_set_function( 1, GPIO_FUNC_SIO);
    gpio_set_function(17, GPIO_FUNC_UART);
    gpio_set_function(16, GPIO_FUNC_UART);
#endif

#if defined(WATCHDOG_PWM) || defined(WATCHDOG_TIMER)
    wrap_watchdog_disable();
#endif

    printf("***** Estação Meteorológica 01 *****\n");
    
    // Inicializar LEDs
    led_rgb_init();

    // Inicializar I2Cs
    i2c_init(         I2C_MAIN_BUS,      I2C_MAIN_BAUDRATE);
    gpio_pull_up(     I2C_MAIN_GPIO_SDA);
    gpio_pull_up(     I2C_MAIN_GPIO_SCL);
    gpio_set_function(I2C_MAIN_GPIO_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_MAIN_GPIO_SCL, GPIO_FUNC_I2C);

    i2c_init(         I2C_OLED_BUS,      I2C_OLED_BAUDRATE);
    gpio_pull_up(     I2C_OLED_GPIO_SCL);
    gpio_pull_up(     I2C_OLED_GPIO_SDA);
    gpio_set_function(I2C_OLED_GPIO_SCL, GPIO_FUNC_I2C);
    gpio_set_function(I2C_OLED_GPIO_SDA, GPIO_FUNC_I2C);
    
    // init display oled
    ui_display_init(false);

    led_rgb_set_st(LEDRGB_ST_BUSY);

#ifndef EASTER_EGG_ENABLED
    // Dump de memória
    if(!gpio_get(BUTTON_B_GPIO)){
        led_rgb_set_st(LEDRGB_ST_WIFI_ST_CONNECTED_WITH_ERROR);
        sleep_ms(10000);
        led_rgb_set_st(LEDRGB_ST_WIFI_ST_DISCONNECTED);

        estation_data_printf();
        stored_printf(true);
        
        led_rgb_set_st(LEDRGB_ST_WIFI_ST_CONNECTED_WITH_ERROR);
        sleep_ms(10000);
        led_rgb_set_st(LEDRGB_ST_BUSY);
    }
#endif

    // verifica se é para configurar:
    bool config = false;
    config |= !gpio_get(BUTTON_A_GPIO);
    config |= !config_data_storage_read();
    if(config) menu_config_init();
    if(config || (!config_data.rtc_with)) menu_clock();

    // Instala e habilita o IRQ do FIFO para o Core 0
    irq_set_exclusive_handler(SIO_IRQ_PROC0, core0_comm_core0_fifo_irq_handler);
    multicore_fifo_clear_irq();//SIO_IRQ_PROC0
    irq_set_enabled(SIO_IRQ_PROC0, true);

    // Ativa Core1  Led fica amarelo durante ativação do core1
    led_rgb_set_st(LEDRGB_ST_WIFI_ST_CONNECTED_WITH_ERROR);
    multicore_fifo_drain();

    printf("Starting Core1\n");
    multicore_launch_core1(core1_main);
    //multicore_fifo_pop_blocking();
    core0_comm_aguarda_cmd();   // Aguarda o Core1 ficar ativo
    led_rgb_set_st(LEDRGB_ST_OFF);

    wifi_init();
    
    uint64_t dt_next_show      =   1000000 * config_data_get_period_in_sec(config_data.period_show);
    uint64_t dt_next_send      =   1000000 * config_data_get_period_in_sec(config_data.period_send);
    uint64_t dt_next_get_time  =   1000000 * config_data_get_period_in_sec(config_data.period_sync_clk);
    uint64_t dt_next_retry     =   1000000 * config_data_get_period_in_sec(config_data.period_retry);

    uint64_t t_next_show       = 0;
    uint64_t t_next_send       = 0;
    uint64_t t_next_get_time   = 0;
    
    WiFiState last_wifi_st = WIFI_ST_DISCONNECTED;
    led_rgb_set_st(LEDRGB_ST_WIFI_ST_DISCONNECTED);

    printf("Starting Core0 Main Loop\n");

#if defined(WATCHDOG_PWM) || defined(WATCHDOG_TIMER)    
    wrap_watchdog_enable();
#endif

    printf("START\n");

    //sleep_ms(10000);

    while (true) {
        bool wifi_st = wifi_update();

#if defined(WATCHDOG_PWM) || defined(WATCHDOG_TIMER)
        wrap_watchdog_refresh();
#endif

        if(wifi_st){
            if(last_wifi_st == WIFI_ST_DISCONNECTED){
                last_wifi_st = WIFI_ST_CONNECTED_NO_ERROR;
                led_rgb_set_st(LEDRGB_ST_WIFI_ST_CONNECTED_NO_ERROR);
            }

            // Atualiza RTC
            if((config_data.period_sync_clk < PERIOD_N)&&(time_us_64() >= t_next_get_time)){
                printf("Requisitando Hora do servidor: ");
                if(get_server_time()){
                    util_print_timestamp(core_comm_new_time);
                    printf(" -- OK --\n");
                    last_wifi_st    = WIFI_ST_CONNECTED_NO_ERROR;
                    led_rgb_set_st(LEDRGB_ST_WIFI_ST_CONNECTED_NO_ERROR);
                    t_next_get_time = time_us_64() + dt_next_get_time; 
                }else{
                    printf(" -- ERRO --\n");
                    last_wifi_st = WIFI_ST_CONNECTED_WITH_ERROR;
                    t_next_get_time = time_us_64() + dt_next_retry; 
                }
            }
                
            // Envio de dados para o servidor
            if((config_data.period_send < PERIOD_N)&&(time_us_64() >= t_next_send)){
                bool fim = false;
                while(!fim){
                    if(core0_comm_give_next_data()){
                        printf("Enviado dados para o servidor: ");
                        estation_data_json(&core_comm_estation_data, estation_data_buf_tmp);
                        printf(estation_data_buf_tmp);
                        
                        if(send_data()){
                            last_wifi_st = WIFI_ST_CONNECTED_NO_ERROR;
                            led_rgb_set_st(LEDRGB_ST_WIFI_ST_CONNECTED_NO_ERROR);
                            core0_comm_give_next_data_sent();
                            printf(" -- OK --\n");
                        }else{
                            fim          = true;
                            last_wifi_st = WIFI_ST_CONNECTED_WITH_ERROR;
                            t_next_send  = time_us_64() + dt_next_retry;
                            printf(" -- ERROR --\n");
                        }
                    }else{
                        fim         = true;
                        t_next_send = time_us_64() + dt_next_send;
                    }
#if defined(WATCHDOG_PWM) || defined(WATCHDOG_TIMER)
                    wrap_watchdog_refresh();
#endif  
                }
            }
        }else{  // WiFi OFF
            last_wifi_st = WIFI_ST_DISCONNECTED;
        }

        // Atualiza os LEDs
        switch(last_wifi_st){
            case WIFI_ST_DISCONNECTED:         led_rgb_set_st(LEDRGB_ST_WIFI_ST_DISCONNECTED);         break;
            case WIFI_ST_CONNECTED_NO_ERROR:   led_rgb_set_st(LEDRGB_ST_WIFI_ST_CONNECTED_NO_ERROR);   break;
            case WIFI_ST_CONNECTED_WITH_ERROR: led_rgb_set_st(LEDRGB_ST_WIFI_ST_CONNECTED_WITH_ERROR); break;
        }

        // Envia dados para o terminal
        if((config_data.period_show < PERIOD_N) && (time_us_64() >= t_next_show)){
            t_next_show = time_us_64() + dt_next_show;

            core0_comm_give_last_data();
            int size  = estation_data_json(&core_comm_estation_data, estation_data_buf_tmp);
            printf("Last Data : %s\n", estation_data_buf_tmp);
        }

#if defined(WATCHDOG_PWM) || defined(WATCHDOG_TIMER)
#ifdef EASTER_EGG_ENABLED        
        if(!gpio_get(BUTTON_J_GPIO)){
            printf("Iniciado processo de Watdog reset\n");
            led_rgb_set_st(LEDRGB_ST_WHITE);
            uint64_t t0 = time_us_64();
            while(true){
                printf("%d\n", (int32_t)(time_us_64() - t0));
                sleep_ms(1000);
            }
        }
#endif
#endif
        
        //cyw43_arch_poll();
        sleep_us(1000);
    }
}
#include <stdio.h>
#include <time.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "hardware/i2c.h"
#include "include/aht10.h"
#include "include/bmp280_main.h"
#include "include/bh1750.h"
#include "include/config.h"
#include "include/config_data.h"
#include "include/core_communication.h"
#include "include/core0_comm.h"
#include "include/core1_main.h"
#include "include/menu_config.h"
#include "include/radio.h"
#include "include/rtc_ds3231.h"
#include "include/storage.h"
#include "include/ui_display.h"


bool falha_de_transmissão = false;

typedef enum{
    STATION_STATE_INIT_CORE0 = 0,
    STATION_STATE_RTC_SINCRONIZE,
    STATION_STATE_TX_STATION_DATA,
    STATION_STATE_INIT_CORE1,
    STATION_STATE_ON_STANDBY,
    STATION_STATE_ON_SENDING_DATA,



}StationState;


void print_time(uint64_t current_time){
    struct tm t;

    gmtime_r((time_t*)&current_time, &t); // Converte para UTC
    char buffer[32];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &t);

    printf("%llu  %s\n", current_time, buffer);
}

void teste_i2c(){
    aht10_init();
    while(1){
        Aht10Data d = aht10_read();
        printf("Temp = %.2f    Humi==%.2f\n", d.temp, d.humidity);

        sleep_ms(100);
    }


    bh1750_init();
    while(1){
        int16_t l = bh1750_read_lux();
        printf("luminosidade = %lu lux\n", l);
    }



    bmp280_main_init();
    while(1){
        sensors_t s;
        s = bmp280_main_get_all_0();
        printf("Temp=%.2f  press=%d  alt=%.1f\n", s.temperature, s.pressure, s.altitude);
        sleep_ms(500);
    }

    rtc_ds3231_test();
    
}














int main()
{

    uint64_t *data_pointer = (uint64_t *)(XIP_BASE + STORAGE_OFFSET_DATA_FIRST);

    uint64_t *data_pointer1;
    uint64_t *data_pointer2;
    uint64_t *data_pointer3;
    uint64_t *data_pointer4;
    uint64_t *data_pointer5;
    uint64_t *data_pointer6;
    data_pointer1 = data_pointer;
    data_pointer2 = data_pointer+1;
    data_pointer3 = data_pointer+3;
    data_pointer4 = data_pointer+32;
    data_pointer6 = data_pointer;
    data_pointer5 = (uint64_t *)(XIP_BASE + STORAGE_OFFSET_DATA_FIRST + 256);
    data_pointer6 +=32;

    stdio_init_all();

#ifdef BITDOGLAB_WITH_DEBUG_PROBE
    gpio_set_function( 0, GPIO_FUNC_SIO);
    gpio_set_function( 1, GPIO_FUNC_SIO);
    gpio_set_function(17, GPIO_FUNC_UART);
    gpio_set_function(16, GPIO_FUNC_UART);
#endif

    printf("***** Estação Metereológica 01 *****\n");
    
    gpio_init(   BUTTON_A_GPIO);
    gpio_init(   BUTTON_B_GPIO);
    gpio_pull_up(BUTTON_A_GPIO);
    gpio_pull_up(BUTTON_B_GPIO);

    gpio_init(   LED_R_GPIO);
    gpio_init(   LED_G_GPIO);
    gpio_init(   LED_B_GPIO);
    gpio_set_dir(LED_R_GPIO, GPIO_OUT);
    gpio_set_dir(LED_G_GPIO, GPIO_OUT);
    gpio_set_dir(LED_B_GPIO, GPIO_OUT);

    gpio_put(    LED_R_GPIO, true);
    sleep_ms(250);
    gpio_put(    LED_R_GPIO, false);
    gpio_put(    LED_G_GPIO, true);
    sleep_ms(250);
    gpio_put(    LED_G_GPIO, false);
    gpio_put(    LED_B_GPIO, true);
    sleep_ms(250);
    gpio_put(    LED_B_GPIO, false);

    // termina a inicialização do núcleo 1
    //ui_display_init();


    // verifica se é para configurar:
    gpio_put(    LED_B_GPIO, true);
    bool config = false;
    config |= !gpio_get(BUTTON_A_GPIO);
    config |= !config_data_storage_read();
    if(config) menu_config_init();
    gpio_put(    LED_B_GPIO, false);


    
    // Instala e habilita o IRQ do FIFO para o Core 0
    irq_set_exclusive_handler(SIO_IRQ_PROC0, core0_comm_core0_fifo_irq_handler);
    irq_set_enabled(SIO_IRQ_PROC0, true);


    gpio_put(    LED_R_GPIO, true);
    multicore_fifo_drain();
    multicore_launch_core1(core1_main);

    // Aguarda o Core1 ficar ativo
    //multicore_fifo_pop_blocking();
    core0_comm_aguarda_cmd();

    gpio_put(    LED_R_GPIO, false);
    gpio_put(    LED_G_GPIO, true);


    radio_init();

    uint64_t dt_next_show      =   1000000 * config_data_get_period_in_sec(config_data.period_show);
    uint64_t dt_next_send      =   1000000 * config_data_get_period_in_sec(config_data.period_send);
    uint64_t dt_next_get_time  =   8000000 * config_data_get_period_in_sec(config_data.period_sync_clk);
    

    uint64_t t_next_show;
    uint64_t t_next_send;
    uint64_t t_next_get_time;
    


    t_next_show     = time_us_64();
    t_next_send     = t_next_show;
    t_next_get_time = t_next_show;

    uint count = 0;
    printf("Start Main Loop\n");
    //while(1);
    while (true) {
        radio_update();
        if(time_us_64() >= t_next_show){
            t_next_show += dt_next_show;
            core0_comm_give_last_data();

            int16_t size = estation_data_json(&core_comm_estation_data, estation_data_buf_tmp);
            //printf(buf);
            //printf("\nSize = %d\n", size);
            //printf("Read Last\n");
        }
        if(time_us_64() >= t_next_send){
            printf("Next Send %d\n", count);

            if(core0_comm_give_next_data()){

                radio_send_data(&core_comm_estation_data);

                int16_t size = estation_data_json(&core_comm_estation_data, estation_data_buf_tmp);
                printf(estation_data_buf_tmp);

                // APOS CONFIRMA ENVIO:
                core0_comm_give_next_data_sent();
            }else{
                t_next_send += dt_next_send;
            }

        }
        if(time_us_64() >= t_next_get_time){
            t_next_get_time += dt_next_get_time;
            //printf("Next time %d\n", count);

            
        }
        count++;
        sleep_us(1000);
    }
}

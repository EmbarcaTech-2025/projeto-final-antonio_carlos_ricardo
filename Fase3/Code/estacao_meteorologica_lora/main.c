#include <stdio.h>
#include "pico/stdlib.h"
#include "include/aq_data.h"
#include "include/code_config.h"
#include "include/flash.h"
#include "include/est_config.h"
#include "include/hw_sleep.h"
#include "include/lora.h"
#include "include/menu_conf.h"

static AqData    aq_data;
static EstConfig est_config;

static void init_ledsrgb_buttons();

int main(){
    stdio_init_all();
    init_ledsrgb_buttons();

    gpio_put(GPIO_LED_RED,   true);
    sleep_ms(10000);    // para dar tempo de conectar a interface serial
    gpio_put(GPIO_LED_GREEN, true);
    printf("********** %s **********\n\n", NAME);
    printf("Version: %s - %s - Build: %s\n\n\n", VERSION, VERSION_DATA, BUILD);

    // se houver erro na leitura oo o botão A estiver prescionado entrar no menu
    if((!flash_conf_read(&est_config)) || (!gpio_get(GPIO_BUTTON_A))){
        menu_conf(&est_config);
    }
    aq_data.control1 = est_config.sensors1;
    gpio_put(GPIO_LED_RED,   false);
    gpio_put(GPIO_LED_GREEN, false);

    int count = 0;
    int ret;
    while (true) {
        // Avisa que vai iniciar a aquisição
        gpio_put(GPIO_LED_BLUE, true);
        printf("Station Wake Up %d\n", count++);
        
        // Faz a aquisição completa, inicia, le, e poem para dormir os sensores
        if(ret = aqdata_init(&aq_data)){
            printf("Init sensors fail: %d\n", ret);
        }
        if(ret = aqdata_read(&aq_data)){
            printf("Read sensors fail: %d\n", ret);
        }
        if(ret = aqdata_sleep(&aq_data)){
            printf("Sleep sensors fail: %d\n", ret);
        }

        // Envia os dados
        lora_send(&est_config, &aq_data);

        // Faz a BitDogLab "dormir"
        printf("Station Sleeping\n");
        gpio_put(GPIO_LED_BLUE, false);
        hw_sleep(est_config.sleep_time_min);
    }
}


static void init_ledsrgb_buttons(){
    // Inicializa GPIOs dos botões
    gpio_init(   GPIO_BUTTON_A);
    gpio_init(   GPIO_BUTTON_B);
    gpio_pull_up(GPIO_BUTTON_A);
    gpio_pull_up(GPIO_BUTTON_B);

    // Inicializa GPIOs dos LEDs
    gpio_init(   GPIO_LED_RED);
    gpio_init(   GPIO_LED_GREEN);
    gpio_init(   GPIO_LED_BLUE);
    gpio_put(    GPIO_LED_RED,   false);
    gpio_put(    GPIO_LED_GREEN, false);
    gpio_put(    GPIO_LED_BLUE,  false);
    gpio_set_dir(GPIO_LED_RED,   GPIO_OUT);
    gpio_set_dir(GPIO_LED_GREEN, GPIO_OUT);
    gpio_set_dir(GPIO_LED_BLUE,  GPIO_OUT);

    // teste dos LEDs
    gpio_put(    GPIO_LED_RED,   true);
    sleep_ms(LED_INIT_DELAY_MS);
    gpio_put(    GPIO_LED_RED,   false);
    gpio_put(    GPIO_LED_GREEN, true);
    sleep_ms(LED_INIT_DELAY_MS);
    gpio_put(    GPIO_LED_GREEN, false);
    gpio_put(    GPIO_LED_BLUE,  true);
    sleep_ms(LED_INIT_DELAY_MS);
    gpio_put(    GPIO_LED_BLUE,  false);
}
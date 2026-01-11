#include <stdio.h>
#include "pico/stdlib.h"
#include "include/aq_data.h"
#include "include/buttons_and_leds.h"
#include "include/code_config.h"
#include "include/flash.h"
#include "include/est_config.h"
#include "include/hw_sleep.h"
#include "include/loop_printf.h"
#include "include/menu_conf.h"
#include "include/wcm.h"


#define MAIN_ST_INIT            LED_YELLOW      // inicializando o sistema
#define MAIN_ST_MENU            LED_RED         // MENU mode
#define MAIN_ST_MENU_AFTER      LED_CYAN
#define MAIN_ST_AQUISITION      LED_BLUE
#define MAIN_ST_TRANSMISSION    LED_WHITE
#define MAIN_ST_SLEEPING        LED_BLACK   // Sleeping


static uint32_t  count = 0;
static AqData    aq_data;
static EstConfig est_config;


int main(){
    stdio_init_all();

    #ifdef ENABLE_GPIO_TEST
    gpio_set_dir(     GPIO_TEST_0, GPIO_OUT);
    gpio_put(         GPIO_TEST_0, 0);
    gpio_set_function(GPIO_TEST_0, GPIO_FUNC_SIO);

    gpio_put(         GPIO_TEST_1, 0);
    gpio_set_dir(     GPIO_TEST_1, GPIO_OUT);
    gpio_set_function(GPIO_TEST_1, GPIO_FUNC_SIO);
    #endif

#ifdef BITDOGLAB_WITH_DEBUG_PROBE
    gpio_set_function( 0, GPIO_FUNC_SIO);
    gpio_set_function( 1, GPIO_FUNC_SIO);
    gpio_set_function(17, GPIO_FUNC_UART);
    gpio_set_function(16, GPIO_FUNC_UART);
#endif
    
    buttons_and_leds_init();
    buttons_and_leds_set_color(MAIN_ST_INIT);
    wcm_init();
    sleep_ms(INITIAL_DELAY_MS);    // para dar tempo de conectar a interface serial

    printf("********** %s **********\n\n", NAME);
    printf("Version: %s - %s - Build: %s\n\n\n", VERSION, VERSION_DATA, BUILD);

    // se houver erro na leitura ou o botão A estiver prescionado entrar no menu
    if((!flash_conf_read(&est_config)) || buttons_and_leds_button_a_pressed()){
        buttons_and_leds_set_color(MAIN_ST_MENU);
        menu_conf(&est_config);
    }

    buttons_and_leds_set_color(MAIN_ST_MENU_AFTER);
    aq_data.active_sensors = est_config.active_sensors;
    hw_sleep_init(est_config.usb_mode, est_config.sleep_time_min);
    
    buttons_and_leds_set_color(LED_BLACK);
    int ret;
    while (true) {
        // Avisa que vai iniciar a aquisição
        if(est_config.leds_on) buttons_and_leds_set_color(MAIN_ST_AQUISITION);
        
        // Faz a aquisição completa, inicia, le, e poem para dormir os sensores
        if(ret = aqdata_init(&aq_data)){
            loop_printf("Init sensors fail: %d\n", ret);
        }
        if(ret = aqdata_read(&aq_data)){
            loop_printf("Read sensors fail: %d\n", ret);
        }
        if(ret = aqdata_sleep(&aq_data)){
            loop_printf("Sleep sensors fail: %d\n", ret);
        }
        //sleep_ms(1000);

        // Envia os dados
        loop_printf("Station sending data\n");
        if(est_config.leds_on) buttons_and_leds_set_color(MAIN_ST_TRANSMISSION);
        //wcm_send(&est_config, &aq_data);
        sleep_ms(1000);

        // Faz a BitDogLab "dormir"
        loop_printf("Station sleeping\n");
        if(est_config.leds_on) buttons_and_leds_set_color(MAIN_ST_SLEEPING);
        hw_sleep();

        loop_printf("Station Wake Up %d\n", ++count);
    }
}
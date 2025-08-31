#include <pico/cyw43_arch.h>
#include "../include/config.h"
#include "../include/led_rgb.h"

static LedRgbState last_state;

void led_rgb_init(){
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

    last_state = LEDRGB_ST_OFF;
}

void led_rgb_set_st(LedRgbState st){
    if(st == last_state) return;

    switch(st){
        case LEDRGB_ST_WIFI_ST_DISCONNECTED:         gpio_put(LED_R_GPIO, true ); gpio_put(LED_G_GPIO, false); gpio_put(LED_B_GPIO, false); break;
        case LEDRGB_ST_WIFI_ST_CONNECTED_NO_ERROR:   gpio_put(LED_R_GPIO, false); gpio_put(LED_G_GPIO, true ); gpio_put(LED_B_GPIO, false); break;
        case LEDRGB_ST_WIFI_ST_CONNECTED_WITH_ERROR: gpio_put(LED_R_GPIO, true ); gpio_put(LED_G_GPIO, true ); gpio_put(LED_B_GPIO, false); break;
        case LEDRGB_ST_BUSY:                         gpio_put(LED_R_GPIO, false); gpio_put(LED_G_GPIO, false); gpio_put(LED_B_GPIO, true ); break;
        //case LEDRGB_ST_OFF
        default: st = LEDRGB_ST_OFF;                 gpio_put(LED_R_GPIO, false); gpio_put(LED_G_GPIO, false); gpio_put(LED_B_GPIO, false); break;        
    }
    last_state = st;
}
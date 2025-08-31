#ifndef LED_RGB_H
#define LED_RGB_H


typedef enum{
    LEDRGB_ST_WIFI_ST_DISCONNECTED = 0,
    LEDRGB_ST_WIFI_ST_CONNECTED_NO_ERROR,
    LEDRGB_ST_WIFI_ST_CONNECTED_WITH_ERROR,
    LEDRGB_ST_BUSY,
    LEDRGB_ST_OFF
} LedRgbState;


void led_rgb_init();
void led_rgb_set_st(LedRgbState st);

#endif // LED_RGB_H
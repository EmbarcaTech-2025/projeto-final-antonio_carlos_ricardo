#ifndef BUTTONS_AND_LEDS_H
#define BUTTONS_AND_LEDS_H

typedef enum{
    LED_BLACK = 0,
    LED_RED,
    LED_GREEN,
    LED_BLUE,
    LED_YELLOW,
    LED_MAGENTA,
    LED_CYAN,
    LED_WHITE
}LedsColor;


void buttons_and_leds_init();
void buttons_and_leds_set_color(LedsColor color);
bool buttons_and_leds_button_a_pressed();


#endif // BUTTONS_AND_LEDS_H
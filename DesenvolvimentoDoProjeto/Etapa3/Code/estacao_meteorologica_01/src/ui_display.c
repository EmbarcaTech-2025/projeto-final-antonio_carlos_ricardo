#include "pico/stdlib.h"
#include "src/oled/display.h"
#include "../include/ui_display.h"

void ui_display_init(){
    display_init();
    for(int i=0;i<3;i++){
        display_fill(true, true);
        sleep_ms(250);
        display_fill(false, true);
        sleep_ms(250);
    }
    //                       "1234567890123456"
    display_msg(false, 0, 0, "   BITDOGLAB");
    display_msg(false, 0, 2, "    ESTACAO");
    display_msg(false, 0, 3, " METEREOLOGICA");
    display_msg(false, 0, 4, "       01");
    display_msg(true,  0, 7, "BUILD:      0000");
    sleep_ms(2500);
    display_fill(false, true);

}
void ui_display_update(){

}
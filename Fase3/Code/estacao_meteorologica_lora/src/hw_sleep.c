#include "../include/hw_sleep.h"

#define HW_SLEEP_WAIT_TO_SLEEP_MS       50

void hw_sleep(uint16_t sleep_minutes){
    sleep_ms(HW_SLEEP_WAIT_TO_SLEEP_MS);
    sleep_ms(1000);
}
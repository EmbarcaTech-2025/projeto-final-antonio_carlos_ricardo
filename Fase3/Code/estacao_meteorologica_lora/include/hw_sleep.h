#ifndef HW_SLEEP_H
#define HW_SLEEP_H

#include "pico/stdlib.h"

void hw_sleep_init();
void hw_sleep_min(uint16_t sleep_minutes);


#endif // HW_SLEEP_H
#ifndef HW_SLEEP_H
#define HW_SLEEP_H

#include "pico/stdlib.h"
#include "est_config.h"

void hw_sleep_init(UsbMode usb_mode, bool ad_on, uint16_t sleep_minutes);
void hw_sleep();


#endif // HW_SLEEP_H
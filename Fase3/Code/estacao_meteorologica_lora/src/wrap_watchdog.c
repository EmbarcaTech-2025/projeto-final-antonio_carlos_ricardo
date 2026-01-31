#include "hardware/watchdog.h"

#include "../include/code_config.h"
#include "../include/wrap_watchdog.h"


void wrap_watchdog_disable(){
    if(!WATCHDOG_ENABLED) return;
    watchdog_disable();
}

//watchdog_disable();
void wrap_watchdog_enable(){
    if(!WATCHDOG_ENABLED) return;
    watchdog_enable(5000, 1);
    watchdog_update();
}

// Necessario no sleep, na leitura do 
void wrap_watchdog_update(){
    if(!WATCHDOG_ENABLED) return;
    watchdog_update();
}

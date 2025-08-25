#include "pico/stdlib.h"
#include "../include/core_communication.h"

uint64_t     core_comm_new_time;
EstationData core_comm_estation_data;

bool core_comm_flash_busy = false;
bool core_comm_core0_busy = true;
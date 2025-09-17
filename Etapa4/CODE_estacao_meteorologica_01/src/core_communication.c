/**
 * @file    core_communication.h
 * @author  Carlos e Ricardo
 * @brief   Comandos e Flags utilizados na comunicação entre os COREs
 * @version 0.1
 * @date    2025-09-05
 * 
 * @copyright Copyright (c) 2025
 */
#include "pico/stdlib.h"
#include "../include/core_communication.h"

// Dados que são trocados entre os COREs
uint64_t     core_comm_new_time;
EstationData core_comm_estation_data;

// Flags para comunicação entre os COREs
bool core_comm_flash_busy = false;
bool core_comm_core0_busy = true;
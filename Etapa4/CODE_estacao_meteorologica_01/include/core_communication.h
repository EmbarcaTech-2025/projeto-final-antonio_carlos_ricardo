/**
 * @file    core_communication.h
 * @author  Carlos e Ricardo
 * @brief   Comandos e Flags utilizados na comunicação entre os COREs
 * @version 0.1
 * @date    2025-09-05
 * 
 * @copyright Copyright (c) 2025
 */
#ifndef CORE_COMMUNICATION_H
#define CORE_COMMUNICATION_H

#include "estation_data.h"

/**
 * @brief Comandos utilizados
 * 
 */
typedef enum{
    CORE_COMM_CMF_NO_CMD = 0,
    CORE_COMM_CMD_NO_MSG_ACK,           // Core1 avisa core0: avisa que não há data para enviar, apenas um ACK
    CORE_COMM_CMD_NEW_TIME,             // Core0 avisa core1: que recebeu uma atualização de time com o valor de: core_comm_new_time
    CORE_COMM_CMD_SENT_DATA,            // Core0 avisa core1: que conseguiu enviar a última MSG
    CORE_COMM_CMD_GIVE_NEXT_SEND_DATA,  // Core0 avisa core1: que ele quer uma nova mensagem para enviar
    CORE_COMM_CMD_GIVE_LAST_DATA,       // Core0 avisa core1: que ele quer a última data para mostrar
    CORE_COMM_CMD_DATA_READY,           // Core1 avisa core0: avisa que o dado para envio está pronto
    CORE_COMM_CMD_HOLD                  // Core1 avisa core0: avisa que o core precisa rodar apenas em RAM, pois a flash será manipulada
} CoreCommunicationCmd;

// Dados que são trocados entre os COREs
extern uint64_t     core_comm_new_time;
extern EstationData core_comm_estation_data;


// Flags para comunicação entre os COREs
extern bool core_comm_flash_busy;
extern bool core_comm_core0_busy;


#endif // CORE_COMMUNICATION_H
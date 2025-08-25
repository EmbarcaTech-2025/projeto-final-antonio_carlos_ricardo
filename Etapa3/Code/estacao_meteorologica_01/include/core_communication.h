#ifndef CORE_COMMUNICATION_H
#define CORE_COMMUNICATION_H

#include "estation_data.h"

typedef enum{
    CORE_COMM_CMF_NO_CMD = 0,
    CORE_COMM_CMD_NO_MSG_ACK,       // Core1 avisa core0: avisa que não há data para enviar, apenas um ACK
    CORE_COMM_CMD_NEW_TIME,         // Core0 avisa core1: que recebeu uma atualização de time com o valor de: core_comm_new_time
    CORE_COMM_CMD_SENT_DATA,        // Core0 avisa core1: que conseguiu enviar a última MSG
    CORE_COMM_CMD_GIVE_NEXT_SEND_DATA,   // Core0 avisa core1: que ele quer uma nova mensagem para enviar
    CORE_COMM_CMD_GIVE_LAST_DATA,   // Core0 avisa core1: que ele quer a última data para mostra
    CORE_COMM_CMD_DATA_READY,       // Core1 avisa core0: avisa que o dado para envio esta pronto
    CORE_COMM_CMD_HOLD
} CoreCommunicationCmd;

extern uint64_t     core_comm_new_time;
extern EstationData core_comm_estation_data;


extern bool core_comm_flash_busy;
extern bool core_comm_core0_busy;


#endif // CORE_COMMUNICATION_H
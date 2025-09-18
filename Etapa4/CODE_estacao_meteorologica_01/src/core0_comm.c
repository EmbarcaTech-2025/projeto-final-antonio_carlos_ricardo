/**
 * @file    core0_comm.c
 * @author  Carlos e Ricardo
 * @brief   Módulo para envio e recebimento de mensagens e dados do Core1
 * @version 0.1
 * @date    2025-09-16
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "../include/config.h"
#include "../include/core_communication.h"
#include "../include/core0_comm.h"


// ---------- Handler de interrupção no Core 0 (em RAM) ----------
static void __not_in_flash_func(core0_hold)(void) {
 
    // Limpa a interrupção manualmente
    //sio_hw->nvic_soft_clear = 1u << SIO_IRQ_PROC0;

    // Desabilita interrupções globais e entra em modo seguro
    uint32_t ints = save_and_disable_interrupts();

    bool r = gpio_get(LED_R_GPIO);
    bool g = gpio_get(LED_G_GPIO);
    bool b = gpio_get(LED_B_GPIO);

    gpio_put(LED_R_GPIO, false);
    gpio_put(LED_G_GPIO, false);
    gpio_put(LED_B_GPIO, true);

    core_comm_core0_busy = false;
    // Espera sinal de finalização em RAM
    while (core_comm_flash_busy) {
        tight_loop_contents();
    }

    core_comm_core0_busy = true;
    gpio_put(LED_R_GPIO, r);
    gpio_put(LED_G_GPIO, g);
    gpio_put(LED_B_GPIO, b);


    // Restaura interrupções antes de sair
    restore_interrupts(ints);
}


/**
 * @brief Variavel com o último comand valido enviado pelo Corr1
 * 
 */
static CoreCommunicationCmd last_core_communication_cmd = CORE_COMM_CMF_NO_CMD;

/**
 * @brief Aguarda até que o Core1 retorne alguma mensagem
 * 
 * @return CoreCommunicationCmd, mensagem do core1 (!= CORE_COMM_CMF_NO_CMD)
 */
CoreCommunicationCmd core0_comm_aguarda_cmd(void){
    while(last_core_communication_cmd == CORE_COMM_CMF_NO_CMD) sleep_us(100);

    CoreCommunicationCmd ret    = last_core_communication_cmd;
    last_core_communication_cmd = CORE_COMM_CMF_NO_CMD;
    return ret;
}

/**
 * @brief Recepiciona todas as mensagens provenientes do Core1,
 * principalmente para tratar prontamente a requisição: CORE_COMM_CMD_HOLD
 * 
 */

// resolve o problema de travamento da Flash
void __not_in_flash_func(core0_comm_core0_fifo_irq_handler)(void) {
//void core0_comm_core0_fifo_irq_handler(void) {
    CoreCommunicationCmd cmd = multicore_fifo_pop_blocking();

    if(cmd == CORE_COMM_CMD_HOLD){
        core0_hold();
    }else{
        last_core_communication_cmd = cmd;
    }

    multicore_fifo_clear_irq();     // Limpa o flag da interrupção
}


/**
 * @brief Avisa o core1 que há uma data/hora de atualização válida,
 * para atualizar o RTC
 * 
 * @param new_time 
 */
void core0_comm_new_time(uint64_t new_time){
    // carrega o novo time na memória compartilhada
    core_comm_new_time = new_time;

    // avisa o core1 que há novo time
    multicore_fifo_push_blocking(CORE_COMM_CMD_NEW_TIME);

    // aguarda o Core1 tratar a info
    core0_comm_aguarda_cmd();
}

/**
 * @brief Solicita para o Core1 o último dado lido
 * só retorna quando o Core1 responder
 * 
 * @return true, há um dado valido na memoria de transferência 
 * @return false, há um dado valido.
 */
bool core0_comm_give_last_data(){
    // Solicita novo dado
    multicore_fifo_push_blocking(CORE_COMM_CMD_GIVE_LAST_DATA);

    // aguarda a resposta e analisa se há ou não novo dado
    return (core0_comm_aguarda_cmd() == CORE_COMM_CMD_DATA_READY);
}

/**
 * @brief Solicita para o Core1 o próximo dado a ser enviado para o servidor
 * 
 * @return true, há um dado valido na memoria de transferência 
 * @return false, não há dado para enviar para o servidor 
 */
bool core0_comm_give_next_data(){
    // Solicita novo dado
    multicore_fifo_push_blocking(CORE_COMM_CMD_GIVE_NEXT_SEND_DATA);

    // aguarda a resposta e analisa se há ou não novo dado
    return (core0_comm_aguarda_cmd() == CORE_COMM_CMD_DATA_READY);
}

/**
 * @brief Avisa ao Core1 que o dado presente na memória foi transferido com sucesso para o servidor
 * 
 */
void core0_comm_give_next_data_sent(){
    // Solicita novo dado
    multicore_fifo_push_blocking(CORE_COMM_CMD_SENT_DATA);

    core0_comm_aguarda_cmd();
}
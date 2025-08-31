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






static CoreCommunicationCmd last_core_communication_cmd = CORE_COMM_CMF_NO_CMD;

CoreCommunicationCmd core0_comm_aguarda_cmd(void){
    while(last_core_communication_cmd == CORE_COMM_CMF_NO_CMD) sleep_us(100);
    CoreCommunicationCmd ret = last_core_communication_cmd;
    last_core_communication_cmd = CORE_COMM_CMF_NO_CMD;
    return ret;
}

void core0_comm_core0_fifo_irq_handler(void) {
    CoreCommunicationCmd cmd = multicore_fifo_pop_blocking();

    if(cmd == CORE_COMM_CMD_HOLD){
        core0_hold();
    }else{
        last_core_communication_cmd = cmd;
    }

    // Limpa flags do SIO (ROE/WOF etc.) para baixar o IRQ
    multicore_fifo_clear_irq();
}





void core0_comm_new_time(uint64_t new_time){
    // carrega o novo time
    core_comm_new_time = new_time;

    // avisa o core1 que há novo time
    multicore_fifo_push_blocking(CORE_COMM_CMD_NEW_TIME);

    // aguarda o Core1 tratar a info
    core0_comm_aguarda_cmd();
    //multicore_fifo_pop_blocking();
}
bool core0_comm_give_last_data(){
    // Solicita novo dado
    multicore_fifo_push_blocking(CORE_COMM_CMD_GIVE_LAST_DATA);

    // aguarda a resposta e analisa se há ou não novo dado
    //return (multicore_fifo_pop_blocking() == CORE_COMM_CMD_DATA_READY);
    return (core0_comm_aguarda_cmd() == CORE_COMM_CMD_DATA_READY);
}
bool core0_comm_give_next_data(){
    // Solicita novo dado
    multicore_fifo_push_blocking(CORE_COMM_CMD_GIVE_NEXT_SEND_DATA);

    // aguarda a resposta e analisa se há ou não novo dado
    //return (multicore_fifo_pop_blocking() == CORE_COMM_CMD_DATA_READY);
    return (core0_comm_aguarda_cmd() == CORE_COMM_CMD_DATA_READY);
}

void core0_comm_give_next_data_sent(){
    // Solicita novo dado
    multicore_fifo_push_blocking(CORE_COMM_CMD_SENT_DATA);

    core0_comm_aguarda_cmd();
    //return (core0_comm_aguarda_cmd() == CORE_COMM_CMF_NO_CMD);
}
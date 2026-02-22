/**
 * @file    core1_main.c
 * @author  your name (you@domain.com)
 * @brief   Inicializa o core1 e mantem o main loop do core1
 * @version 0.1
 * @date    2025-09-16
 * 
 * @copyright Copyright (c) 2025
 */
#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "../include/ad_board.h"
#include "../include/config_data.h"
#include "../include/core_communication.h"
#include "../include/core0_comm.h"
#include "../include/core1_acquisition.h"
#include "../include/core1_data.h"
#include "../include/core1_data_mem.h"
#include "../include/core1_main.h"
#include "../include/estation_data.h"
#include "../include/rtc_wrap.h"
#include "../include/storage.h"
#include "../include/storage_adm.h"
#include "../include/ui_display.h"
#include "../lib/oled/display.h"

/**
 * @brief Inicializa o core1 e mantem o main loop do core1
 * o main loop:
 *  - escalona as aquisições
 *  - atualiza o display
 *  - monitora o Joystick para saber o que deve mostrar no display
 *  - trata as mensagens/requisições recebidas do core0
 */
void core1_main(){
    multicore_fifo_drain();

    // Inicializa flash, le last cycle;
    uint64_t next_cycle = storage_adm_init();

    // inicializa periféricos
    core1_acquisition_init(next_cycle);

    // Desnecessário, ma ajuda na detecção que o core1 está sendo ativado
    sleep_ms(3000);

    display_fill(false, true);
    // Comunica que o Core1 está ativo
    multicore_fifo_push_blocking(CORE_COMM_CMD_NO_MSG_ACK);


    uint32_t aq_n_cycle           = config_data_get_period_in_sec(config_data.period_acquisition); 
    uint32_t aq_cycle             = 0;
    uint64_t t_next_aquisition    = time_us_64();
    int32_t last_send_index       = -1;
    uint64_t t_next_read_joystick = 0;
    while(1){
        if(time_us_64() >= t_next_aquisition){
            core1_acquisition_get();

            if((++aq_cycle) >= aq_n_cycle){
                aq_cycle = 0;

                // salvar dado
                uint32_t index = storage_adm_get_next_aq_index();
                core1_data_mem_save(index, &core1_estation_data);
            }
            t_next_aquisition += 1000000;
        }

        int8_t delta = 0;
        if(time_us_64() >= t_next_read_joystick){
            uint16_t ad = ad_boad_get_joystick_y();
            if(ad > 3072){
                delta = -1;
                t_next_read_joystick = time_us_64() + 500000;
            }else if (ad < 1024){
                delta = 1;
                t_next_read_joystick = time_us_64() + 500000;
            }
        }

        ui_display_update(&core1_estation_data, delta);

        if(multicore_fifo_rvalid()){
            uint32_t cmd = multicore_fifo_pop_blocking();
            switch(cmd){
                case CORE_COMM_CMD_NO_MSG_ACK:
                    multicore_fifo_push_blocking(CORE_COMM_CMD_NO_MSG_ACK);
                    break;
                case CORE_COMM_CMD_NEW_TIME: // Core0 avisa core1: que recebeu uma atualização de time com o valor de: core_comm_new_time
                    //atualizar time
                    rtc_wrap_set(core_comm_new_time);
                    multicore_fifo_push_blocking(CORE_COMM_CMD_NO_MSG_ACK);
                    break;
                case CORE_COMM_CMD_SENT_DATA:        // Core0 avisa core1: que conseguiu enviar a última MSG
                    // dar baixa no envio
                    if(storage_adm_was_sent() && (last_send_index >=0)){
                        stored_data_read(last_send_index, &core_comm_estation_data);

                        core_comm_estation_data.t_send = rtc_wrap_get();

                        // documentar a linha abaixo para não salvar de fato
                        core1_data_mem_update(last_send_index, &core_comm_estation_data);

                        last_send_index = -1;
                    }

                    multicore_fifo_push_blocking(CORE_COMM_CMD_NO_MSG_ACK);
                    break;
                case CORE_COMM_CMD_GIVE_NEXT_SEND_DATA:   // Core0 avisou o core1: que ele quer uma nova mensagem para enviar
                    // obtem o proximo item para enviar para o servidor
                    last_send_index = storage_adm_get_next_send_index();
                    if(last_send_index >= 0){
                        stored_data_read(last_send_index, &core_comm_estation_data);
                        multicore_fifo_push_blocking(CORE_COMM_CMD_DATA_READY);
                    }else{
                        multicore_fifo_push_blocking(CORE_COMM_CMD_NO_MSG_ACK);
                    }
                    break;
                case CORE_COMM_CMD_GIVE_LAST_DATA:   // Core0 avisou o core1: que ele quer a última data para mostra
                    // coloca o último dado na memória compartilhada
                    memcpy(&core_comm_estation_data, &core1_estation_data, sizeof(EstationData));
                    multicore_fifo_push_blocking(CORE_COMM_CMD_DATA_READY);
                    break;
            }
            
        }
    }
}
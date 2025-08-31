#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "../include/ad_board.h"
#include "../include/config_data.h"
#include "../include/core1_acquisition.h"
#include "../include/core_communication.h"
#include "../include/core0_comm.h"
#include "../include/core1_data.h"
#include "../include/core1_main.h"
#include "../include/estation_data.h"
#include "../include/rtc_wrap.h"
#include "../include/storage.h"
#include "../include/storage_adm.h"
#include "../include/ui_display.h"
#include "../lib/oled/display.h"


void core1_main(){
    multicore_fifo_drain();

    // Inicializa flash, le last cycle;
    uint64_t cycle = storage_adm_init();

    // inicializa periféricos
    core1_acquisition_init(cycle);

    sleep_ms(3000);

    display_fill(false, true);
    multicore_fifo_push_blocking(CORE_COMM_CMD_NO_MSG_ACK);


    uint32_t aq_n_cycle        = config_data_get_period_in_sec(config_data.period_acquisition); 
    uint32_t aq_cycle          = 0;
    uint64_t t_next_aquisition = time_us_64();

    int32_t index;
    int32_t last_send_index = -1;
    uint64_t t_next_ad = 0;
    while(1){
        if(time_us_64() >= t_next_aquisition){
            //core1_acquisition_get(next_cycle++);
            core1_acquisition_get();
            if((++aq_cycle) >= aq_n_cycle){
                aq_cycle = 0;

                // salvar dado
                core1_acquisition_save_aq();
            }
            t_next_aquisition += 1000000;
        }

        int8_t delta = 0;
        if(time_us_64() >= t_next_ad){
            uint16_t ad = ad_boad_get_joystick_y();
            if(ad > 3072){
                delta = -1;
                t_next_ad = time_us_64() + 500000;
            }else if (ad < 1024){
                delta = 1;
                t_next_ad = time_us_64() + 500000;
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
                    //index = storage_adm_can_update_lest_send();
                    last_send_index = storage_adm_can_update_lest_send();
                    if(last_send_index >=0){
                        stored_data_read(last_send_index, &core_comm_estation_data);

                        // REVER para colocar o T atual
                        core_comm_estation_data.t_send = core_comm_estation_data.t_aquire;

                        // documentar a linha abaixo para não salvar de fato
                        core1_acquisition_update(&core_comm_estation_data, last_send_index);
                        

                        last_send_index = -1;
                    }else{

                    }

                    multicore_fifo_push_blocking(CORE_COMM_CMD_NO_MSG_ACK);
                    break;
                case CORE_COMM_CMD_GIVE_NEXT_SEND_DATA:   // Core0 avisa core1: que ele quer uma nova mensagem para enviar
                    last_send_index = storage_adm_get_next_sent();
                    if(last_send_index >= 0){
                        stored_data_read(last_send_index, &core_comm_estation_data);
                        multicore_fifo_push_blocking(CORE_COMM_CMD_DATA_READY);
                    }else{
                        multicore_fifo_push_blocking(CORE_COMM_CMD_NO_MSG_ACK);
                    }
                    break;
                case CORE_COMM_CMD_GIVE_LAST_DATA:   // Core0 avisa core1: que ele quer a última data para mostra
                    memcpy(&core_comm_estation_data, &core1_estation_data, sizeof(EstationData));
                    multicore_fifo_push_blocking(CORE_COMM_CMD_DATA_READY);
                    break;
                //CORE_COMM_CMD_NO_DATA,          // Core1 avisa core0: avisa que não há data para enviar
                //CORE_COMM_CMD_DATA_READY,       // Core1 avisa core0: avisa que o dado para envio esta pronto
            }
            
        }



        // Core 1 retornou alguma info?
    }

}
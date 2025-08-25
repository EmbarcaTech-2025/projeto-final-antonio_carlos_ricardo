#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "../include/config_data.h"
#include "../include/core1_acquisition.h"
#include "../include/core_communication.h"
#include "../include/core0_comm.h"
#include "../include/core1_data.h"
#include "../include/core1_main.h"
#include "../include/rtc_ds3231.h"
#include "../include/storage.h"
#include "../include/storage_adm.h"


void core1_main(){
    multicore_fifo_drain();

    // Inicializa flash, le last cycle;
    uint64_t cycle = storage_adm_init();

    // inicializa periféricos
    core1_acquisition_init(cycle);

    sleep_ms(3000);
    multicore_fifo_push_blocking(CORE_COMM_CMD_NO_MSG_ACK);


    uint32_t aq_n_cycle        = config_data_get_period_in_sec(config_data.period_acquisition); 
    uint32_t aq_cycle          = 0;
    uint64_t t_next_aquisition = time_us_64();

    int32_t index;
    while(1){
        if(time_us_64() >= t_next_aquisition){
            //core1_acquisition_get(next_cycle++);
            core1_acquisition_get();
            if((++aq_cycle) >= aq_n_cycle){
                aq_cycle = 0;

                // salvar dado
                core1_acquisition_save();
            }
            t_next_aquisition += 1000000;
        }

        if(multicore_fifo_rvalid()){
            uint32_t cmd = multicore_fifo_pop_blocking();
            switch(cmd){
                case CORE_COMM_CMD_NO_MSG_ACK:
                    multicore_fifo_push_blocking(CORE_COMM_CMD_NO_MSG_ACK);
                    break;
                case CORE_COMM_CMD_NEW_TIME: // Core0 avisa core1: que recebeu uma atualização de time com o valor de: core_comm_new_time
                    //atualizar time
                    rtc_ds3231_set(core_comm_new_time);
                    multicore_fifo_push_blocking(CORE_COMM_CMD_NO_MSG_ACK);
                    break;
                case CORE_COMM_CMD_SENT_DATA:        // Core0 avisa core1: que conseguiu enviar a última MSG
                    // dar baixa no envio
                    index = storage_adm_can_update_lest_send();
                    if(index >=0){

                    }else{

                    }

                    multicore_fifo_push_blocking(CORE_COMM_CMD_NO_MSG_ACK);
                    break;
                case CORE_COMM_CMD_GIVE_NEXT_SEND_DATA:   // Core0 avisa core1: que ele quer uma nova mensagem para enviar
                    index = storage_adm_get_next_sent();
                    if(index >= 0){
                        multicore_fifo_push_blocking(CORE_COMM_CMD_DATA_READY);
                        stored_data_read(index, &core_comm_estation_data);
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
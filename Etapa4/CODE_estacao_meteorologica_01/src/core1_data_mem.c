/**
 * @file    core1_data_mem.c
 * @author  Carlos e Ricardo
 * @brief   Faz operações de leitura e escrita dos dados da estação na memoria Flash
 * - bloqueia o acesso a flash durante as operações de escrita
 * 
 * @version 0.1
 * @date    2025-09-05
 * 
 * @copyright Copyright (c) 2025
 */
#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "../include/core_communication.h"
#include "../include/core1_data_mem.h"
#include "../include/estation_data.h"
#include "../include/storage.h"



/**
 * @brief Obtem o cycle para um determinado índice
 * 
 * @param index 
 * @return uint64_t 
 */
static uint64_t core1_data_mem_get_cycle(uint32_t index){
    uint64_t buf[3];
    stored_data_read_3_uint64(index, buf);
    return buf[0];
}


/**
 * @brief Salva dados da estação.
 * - Faz o bloqueio do CORE0 e desabilita as interrupções antes da operação
 * 
 * @param index, índice do dado
 * @param estation_data, dado a ser gravado
 * @param erase, se é para limpar a memória antes de escrever, 
 */
static void core1_data_mem_save_safe(uint32_t index, EstationData * estation_data, bool erase){
    /*
    if(erase) printf("SAVING AQ   Index=%4d, cycle=%4d\n", index, estation_data->cycle);
         else printf("UPDATING AQ Index=%4d, cycle=%4d\n", index, estation_data->cycle);
    */

    // configura flags de comunicação com o core0
    core_comm_flash_busy = true;
    core_comm_core0_busy = true;

    // pede para parar o core 0
    multicore_fifo_push_blocking(CORE_COMM_CMD_HOLD);

    // aguarda parada do core0
    while(core_comm_core0_busy);

    // core 0 parou, poso executar

    // Paro minhas interrupções
    uint32_t ints = save_and_disable_interrupts();

    // Executo a escrita
    stored_data_write(index, estation_data, !erase);

    // Restauro minhas interrupções
    restore_interrupts(ints);

    // avisa o core 0 que operação com flash acabou
    core_comm_flash_busy = false;

    // aguarda o core 0 voltar a rodar
    while(!core_comm_core0_busy);
}


/**
 * @brief Lê o dado da estação que está na Flash
 * 
 * @param index, define o local da memoria a onde o dado está
 * @param estation_data, ponteiro para onde copiar o dado
 */
void core1_data_mem_read(uint32_t index, EstationData * estation_data){
    stored_data_read(index, estation_data);
}

/**
 * @brief Salva o dado da estação na Flash
 * - se necessário limpa o bloco de memória antes
 * - bloqueia o acesso a flash durante as operações de escrita
 * 
 * @param index, define o local da memoria a onde o dado será salvo
 * @param estation_data, ponteiro do dado a ser salvo
 */
void core1_data_mem_save(uint32_t index, EstationData * estation_data){
    core1_data_mem_save_safe(index, estation_data, true);
}

/**
 * @brief Atualiza o dado da estação Flash
 * - Utilizado para salvar o t_send
 * - NÃO limpa a memória
 * - Antes de fazer o update verifica se o cycle continua válido
 * 
 * @param index, define o local da memoria a onde o dado será salvo
 * @param estation_data, ponteiro do dado a ser atualizado
 */
void core1_data_mem_update(uint32_t index, EstationData * estation_data){
    // verifica se o registro já não foi sobre escrito ou apagado
    if(estation_data->cycle != core1_data_mem_get_cycle(index)) return;

    core1_data_mem_save_safe(index, estation_data, false);
}
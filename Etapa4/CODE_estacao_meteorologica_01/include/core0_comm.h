/**
 * @file    core0_comm.h
 * @author  Carlos e Ricardo
 * @brief   Módulo para envio e recebimento de mensagens e dados do Core1
 * @version 0.1
 * @date    2025-09-16
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef CORE0_COMM_H
#define CORE0_COMM_H

/**
 * @brief Aguarda até que o Core1 retorne alguma mensagem
 * 
 * @return CoreCommunicationCmd, mensagem do core1 (!= CORE_COMM_CMF_NO_CMD)
 */
CoreCommunicationCmd core0_comm_aguarda_cmd(void);

/**
 * @brief Recepiciona todas as mensagens provenientes do Core1,
 * principalmente para tratar prontamente a requisição: CORE_COMM_CMD_HOLD
 * 
 */
void core0_comm_core0_fifo_irq_handler(void);
/**
 * @brief Avisa o core1 que há uma data/hora de atualização válida,
 * para atualizar o RTC
 * 
 * @param new_time 
 */
void core0_comm_new_time(uint64_t new_time);
/**
 * @brief Solicita para o Core1 o último dado lido
 * só retorna quando o Core1 responder
 * 
 * @return true, há um dado valido na memoria de transferência 
 * @return false, há um dado valido.
 */
bool core0_comm_give_last_data();
/**
 * @brief Solicita para o Core1 o próximo dado a ser enviado para o servidor
 * 
 * @return true, há um dado valido na memoria de transferência 
 * @return false, não há dado para enviar para o servidor 
 */
bool core0_comm_give_next_data();
/**
 * @brief Avisa ao Core1 que o dado presente na memória foi transferido com sucesso para o servidor
 * 
 */
void core0_comm_give_next_data_sent();


#endif // CORE0_COMM_H
/**
 * @file    storage_adm.h
 * @author  Carlos e Ricardo
 * @brief 
 * @version 0.1
 * @date    2025-09-06
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef STORAGE_ADM_H
#define STORAGE_ADM_H

/**
 * @brief Scaneia a Flash para mapear os indices das aquisições válidas de das já transmitidas
 * 
 * @return uint64_t, retorna o próximo cycle que deve ser usado 
 */
uint64_t storage_adm_init();

/**
 * @brief define o indice de onde a próxima aquisição deverá ser salva
 * 
 * @return int32_t 
 */
int32_t  storage_adm_get_next_aq_index();
/**
 * @brief Retorna o indice da aquisição que deve ser enviada para o servidor
 * 
 * @return int32_t, retorna < 0 caso não haja aquisição que deva ser enviada 
 */
int32_t  storage_adm_get_next_send_index();
/**
 * @brief Avisa que a ultima aquisição informada por: storage_adm_get_next_aq_index(); foi transmitida com sucesso
 * @return true, faça o update da aquisição 
 * @return false, não faça o update, pois este endereço da memoria foi utilizado por outra aquisição 
 */
bool     storage_adm_was_sent();



#endif // STORAGE_ADM_H
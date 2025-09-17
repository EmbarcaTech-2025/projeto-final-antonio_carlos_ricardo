/**
 * @file    wifi.h
 * @author  Carlos e Ricardo
 * @brief   Inicializa, Atualiza, Conecta e Desconecta o módulo de WiFi
 * @version 0.1
 * @date    2025-09-05
 */
#ifndef WIFI_H
#define WIFI_H

#include "pico/stdlib.h" 

/**
 * @brief Inicializa variáveis
 * 
 */
void wifi_init();

/**
 * @brief Atualiza o estado do WiFi
 * - Se conectado verifica se continua conectado ao roteador
 * - Se desconectado tenta reconectar ao roteador
 * @return true, está conectado
 * @return false, está desconectado
 */
bool wifi_update();


#endif // WIFI_H


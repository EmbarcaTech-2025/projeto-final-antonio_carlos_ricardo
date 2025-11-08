/**
 * @file    wrap_watchdog.h
 * @author  Carlos e Ricardo
 * @brief   Gerencia o watchdog, criando artificios para que funcione com um timeout de 45 segundos
 * @version 0.1
 * @date    2025-09-18
 * 
 * @copyright Copyright (c) 2025
 */
#ifndef WRAP_WATCHDOG_H
#define WRAP_WATCHDOG_H

/**
 * @brief Disabilita o Watchdog
 * 
 */
void wrap_watchdog_disable();
/**
 * @brief Habilita o Watchdog
 * como o periodo necessário do wathdog é de 45 segundos, e o máximo do watchdo é algo em torno de 7 segundos
 * é utilizada uma função tick para ir fazendo o refresh periodicamente até atingir os 45 segundos
 * para isso há duas opções definidas no config.h 
 *   usando interrupção de PWM
 *   usando interrupção de Timer
 */
void wrap_watchdog_enable();
/**
 * @brief faz o refresh do watchdog
 * 
 */
void wrap_watchdog_refresh();

#endif  // WRAP_WATCHDOG_H
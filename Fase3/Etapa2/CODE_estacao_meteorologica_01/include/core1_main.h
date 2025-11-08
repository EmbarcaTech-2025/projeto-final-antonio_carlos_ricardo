/**
 * @file    core1_main.h
 * @author  your name (you@domain.com)
 * @brief   Inicializa o core1 e mantem o main loop do core1
 * @version 0.1
 * @date    2025-09-16
 * 
 * @copyright Copyright (c) 2025
 */
#ifndef CORE1_MAIN_H
#define CORE1_MAIN_H

/**
 * @brief Inicializa o core1 e mantem o main loop do core1
 * o main loop:
 *  - escalona as aquisições
 *  - atualiza o display
 *  - monitora o Joystick para saber o que deve mostrar no display
 *  - trata as mensagens/requisições recebidas do core0
 */
void core1_main();



#endif // CORE1_MAIN_H
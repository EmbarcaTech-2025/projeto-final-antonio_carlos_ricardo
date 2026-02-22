/**
 * @file    ui_display.h
 * @author  Carlos e Ricardo
 * @brief   Inicializa display OLED e mostra dados da estação
 * @version 0.1
 * @date    2025-09-05
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef UI_DISPLAY_H
#define UI_DISPLAY_H

/**
 * @brief Inicializa Display OLED, e pode fazer checagem visual
 * 
 * @param only_init, se true não faz checagem visual
 */
void ui_display_init(bool only_init);

/**
 * @brief Atualiza display com os dados da estação
 * 
 * @param estation_data, ponteiro dos dados da estação
 * @param change_pos, muda a posição dos dados a serem mostrados, pode ser positivo, negativo ou zero 
 */
void ui_display_update(EstationData * estation_data, int8_t change_pos);


#endif // UI_DISPLAY_H
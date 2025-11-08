/**
 * @file    util.h
 * @author  Carlos e Ricardo
 * @brief   Funções genericas utilizadas em várias partes
 * @version 0.1
 * @date    2025-09-16
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef UTIL_H
#define UTIL_H

/**
 * @brief Gera um prinf de um timestamp
 * 
 * @param timestamp 
 */
void util_print_timestamp(uint64_t timestamp);

/**
 * @brief Preenche um buffer com um timestamp de forma humana
 * 
 * @param timestamp 
 * @param buf32 
 */
void util_string_timestamp(uint64_t timestamp, char *buf32);



#endif // UTIL_H
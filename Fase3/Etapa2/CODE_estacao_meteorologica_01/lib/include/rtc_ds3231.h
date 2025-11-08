/**
 * @file    rtc_ds3231.h
 * @author  Carlos e Ricardo
 * @brief   Lê e atualiza data/hora no rtc ds3231
 * @version 0.1
 * @date    2025-09-17
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef RTC_DS3231_H
#define RTC_DS3231_H


/**
 * @brief Ferramenta auxiliar que gera timestamp apartir de valores de data/hora
 * 
 * @param ano 
 * @param mes 
 * @param dia 
 * @param hora 
 * @param minuto 
 * @param segundo 
 * @return uint64_t 
 */
uint64_t rtc_ds3231_gerar_timestamp(int ano, int mes, int dia, int hora, int minuto, int segundo);
/**
 * @brief Escreve em um buffer(str) o timestamp de forma humana
 * 
 * @param timestamp 
 * @param str 
 */
void     rtc_ds3231_timestamp_to_string(uint64_t timestamp, char * str);
/**
 * @brief Obtem o timestamp do RTC
 * 
 * @return uint64_t 
 */
uint64_t rtc_ds3231_get();
/**
 * @brief Atualiza o RTC com um timestamp
 * 
 * @param timestamp 
 */
void     rtc_ds3231_set(uint64_t timestamp);

/**
 * @brief Rotinas para teste do uso e funcionamento do RTC
 * 
 */
void     rtc_ds3231_test();


#endif // RTC_DS3231_H
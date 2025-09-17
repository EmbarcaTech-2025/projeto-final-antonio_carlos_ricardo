/**
 * @file    rtc_wrap.h
 * @author  Carlos e Ricardo
 * @brief   Lê e atualiza o Clock que pode ser um RTC físico ou um virtual
 * @version 0.1
 * @date    2025-09-05
 * 
 * @copyright Copyright (c) 2025
 */
#ifndef RTC_WRAP_H
#define RTC_WRAP_H


/**
 * @brief Retorna o timestamp atual do Clock
 * 
 * @return uint64_t, timestamp atual do Clock
 */
uint64_t rtc_wrap_get();

/**
 * @brief seta o timestamp atual do Clock
 * 
 * @param timestamp, valor para o set do timestamp atual do Clock
 */
void     rtc_wrap_set(uint64_t timestamp);



#endif // RTC_WRAP_H
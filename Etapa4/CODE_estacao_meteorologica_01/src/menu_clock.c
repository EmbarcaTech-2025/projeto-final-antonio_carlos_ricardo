/**
 * @file    menu_clock.c
 * @author  Carlos e Ricardo
 * @brief   Ativa o Menu para a configuração do relógio RTC(ou não)
 * - O Menu vai funcionar através de um terminal serial via USB
 * @version 0.1
 * @date    2025-09-05
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#include <ctype.h>
#include <stdio.h>
#include <time.h>
#include "pico/stdlib.h"
#include "../include/menu_clock.h"
#include "../include/rtc_wrap.h"
#include "../lib/include/rtc_ds3231.h"


/**
 * @brief Ativa o Menu para a configuração do relógio RTC(ou não)
 * - O Menu vai funcionar através de um terminal serial via USB
 */
void menu_clock(){
    uint64_t timestamp = rtc_wrap_get();
    struct tm t;
    bool fim = false;
    while(!fim){
        gmtime_r((time_t*)&timestamp, &t);

        printf("\033[2J\033[H");    // Faz com que o terminal volte para o início da tela
        printf("***** Configuração do Clock *****\n\n");
        char str[32];
        rtc_ds3231_timestamp_to_string(rtc_wrap_get(), str);
        printf("Valor atual: %s\n\n", str);
        printf("1+ 2-) Ano: %4d\n",     t.tm_year - 100 + 2000);
        printf("3+ 4-) Mes:   %2d\n",   t.tm_mon + 1);
        printf("5+ 6-) Dia:   %2d\n\n", t.tm_mday);
        printf("7+ 8-) Hora: %2d\n",    t.tm_hour);
        printf("9+ 0-) Min : %2d\n",    t.tm_min);
        printf("A+ B-) Sec : %2d\n\n",  t.tm_sec);

        printf("S) Save\n\n");
        printf("X) Sair (eXit)\n");

        switch(toupper(getchar())){
            case '1': t.tm_year++; break;
            case '2': t.tm_year--; break;

            case '3': t.tm_mon++; break;
            case '4': t.tm_mon--; break;

            case '5': t.tm_mday++; break;
            case '6': t.tm_mday--; break;

            case '7': t.tm_hour++; break;
            case '8': t.tm_hour--; break;

            case '9': t.tm_min++; break;
            case '0': t.tm_min--; break;

            case 'A': t.tm_sec++; break;
            case 'B': t.tm_sec--; break;

            case 'S':
                rtc_wrap_set(timestamp) ;
                break;
            case 'X': fim = true; break;
        }

        timestamp = rtc_ds3231_gerar_timestamp(t.tm_year - 100 + 2000, t.tm_mon + 1, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec);
    }
    printf("\n***** FIM da Configuração do Clock *****\n\n");
}

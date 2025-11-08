/**
 * @file    wrap_watchdog.h
 * @author  Carlos e Ricardo
 * @brief   Gerencia o watchdog, criando artificios para que funcione com um timeout de 45 segundos
 * @version 0.1
 * @date    2025-09-18
 * 
 * @copyright Copyright (c) 2025
 */
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/timer.h"
#include "hardware/watchdog.h"
#include "../include/config.h"
#include "../include/wrap_watchdog.h"


static volatile int16_t watchdog_count;

#ifdef WATCHDOG_TIMER

// Callback de interrupção do timer
static bool watchdog_feed_callback(repeating_timer_t *rt) {
    if(watchdog_count > 0){
        watchdog_update(); // alimenta o watchdog
        watchdog_count--;
    }
    return true; // retorna true para repetir o timer
}

/**
 * @brief Disabilita o Watchdog
 * 
 */
void wrap_watchdog_disable(){
    watchdog_disable();
    watchdog_update();
}

/**
 * @brief Habilita o Watchdog
 * como o periodo necessário do wathdog é de 45 segundos, e o máximo do watchdo é algo em torno de 7 segundos
 * é utilizada uma função tick para ir fazendo o refresh periodicamente até atingir os 45 segundos
 * para isso há duas opções definidas no config.h 
 *   usando interrupção de PWM
 *   usando interrupção de Timer
 */
void wrap_watchdog_enable(){
    static repeating_timer_t timer_watchdog;
    add_repeating_timer_ms(1000, watchdog_feed_callback, NULL, &timer_watchdog);
    watchdog_enable(5000, 1);
    wrap_watchdog_refresh();
}

/**
 * @brief faz o refresh do watchdog
 * 
 */
void wrap_watchdog_refresh(){
    watchdog_count = 45;
    watchdog_update();
}
#endif

#ifdef WATCHDOG_PWM

#define UI_PWM_TIME_TICK_SLICE     4
#define UI_PWM_TIME_TICK_DIVISOR 125
#define UI_PWM_TIME_TICK_WRAP   9999    // 100Hz

void static pwm_tick(){
    pwm_clear_irq(UI_PWM_TIME_TICK_SLICE);
    if(watchdog_count > 0){
        watchdog_update(); // alimenta o watchdog
        watchdog_count--;
    }
}

/**
 * @brief Disabilita o Watchdog
 * 
 */
void wrap_watchdog_disable(){
    watchdog_disable();
}

/**
 * @brief Habilita o Watchdog
 * como o periodo necessário do wathdog é de 45 segundos, e o máximo do watchdo é algo em torno de 7 segundos
 * é utilizada uma função tick para ir fazendo o refresh periodicamente até atingir os 45 segundos
 * para isso há duas opções definidas no config.h 
 *   usando interrupção de PWM
 *   usando interrupção de Timer
 */
void wrap_watchdog_enable(){
    pwm_set_clkdiv_int_frac(  UI_PWM_TIME_TICK_SLICE, UI_PWM_TIME_TICK_DIVISOR, 0);
    pwm_set_wrap(             UI_PWM_TIME_TICK_SLICE, UI_PWM_TIME_TICK_WRAP);
    pwm_clear_irq(            UI_PWM_TIME_TICK_SLICE);
    pwm_set_irq_enabled(      UI_PWM_TIME_TICK_SLICE, true);
    irq_set_exclusive_handler(PWM_IRQ_WRAP, pwm_tick);
    irq_clear(                PWM_IRQ_WRAP);
    irq_set_enabled(          PWM_IRQ_WRAP, true);
    pwm_set_enabled(          UI_PWM_TIME_TICK_SLICE, true);

    watchdog_enable(5000, 1);
    wrap_watchdog_refresh();
}

/**
 * @brief faz o refresh do watchdog
 * 
 */
void wrap_watchdog_refresh(){
    watchdog_count = 4500;
    watchdog_update();
}
#endif


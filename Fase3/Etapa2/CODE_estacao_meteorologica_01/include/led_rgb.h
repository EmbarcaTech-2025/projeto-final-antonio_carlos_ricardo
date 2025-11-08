/**
 * @file    led_rgb.h
 * @author  Carlos e Ricardo
 * @brief   Configura portas GPIO dos LEDs RGB e botões, controla LEDs
 * @version 0.1
 * @date    2025-09-05
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef LED_RGB_H
#define LED_RGB_H

/**
 * @brief Estados pré definidos dos LEDs RGB
 * 
 */
typedef enum{
    LEDRGB_ST_WIFI_ST_DISCONNECTED = 0,     // Red
    LEDRGB_ST_WIFI_ST_CONNECTED_NO_ERROR,   // Green
    LEDRGB_ST_WIFI_ST_CONNECTED_WITH_ERROR, // Yello
    LEDRGB_ST_BUSY,                         // Blue
    LEDRGB_ST_WHITE,                        // White
    LEDRGB_ST_OFF                           // Black
} LedRgbState;

/**
 * @brief Configura portas GPIO dos LEDs RGB e botões
 * - Faz um rápido teste nos LEDs
 */
void led_rgb_init();

/**
 * @brief Configura os LEDs RGB com o estado pré-definido
 * 
 * @param st, estado dos LEDs 
 */
void led_rgb_set_st(LedRgbState st);

#endif // LED_RGB_H
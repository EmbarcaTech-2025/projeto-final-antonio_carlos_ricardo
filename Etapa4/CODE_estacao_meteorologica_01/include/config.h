/**
 * @file    config.h
 * @author  Carlos e Ricardo
 * @brief   Configurações do hardware e definição de mensagens
 * @version 0.1
 * @date    2025-09-16
 */
#ifndef CONFIG_H
#define CONFIG_H


#define DEBUG_BITDOG_LAB
#define BITDOGLAB_WITH_DEBUG_PROBE

#define WATCHDOG_ENABLED
#define EASTER_EGG_ENABLED


#define SW_BUILD "0015"     // 4 CARACTERES


#define DEBUG_PRINTF_ENABLE      true

#define DEB_PR_CORE0                (DEBUG_PRINTF_ENABLE && true) 
#define DEB_PR_CORE1                (DEBUG_PRINTF_ENABLE && true) 

#define DEB_PR_CORE0_UPDATE_TIME    (DEB_PR_CORE0 && true)
#define DEB_PR_CORE0_SEND_DATA      (DEB_PR_CORE0 && true)
#define DEB_PR_CORE0_SHOW           (DEB_PR_CORE0 && true)

#define DEB_PR_CORE0_WIFI_SEND_DATA (DEB_PR_CORE0 && false)
#define DEB_PR_CORE0_WIFI_GET_TIME  (DEB_PR_CORE0 && true)

#define DEB_PR_CORE1_AQUISITION     (DEB_PR_CORE1 && true) 
#define DEB_PR_CORE1_SAVE_DATA      (DEB_PR_CORE1 && true) 



#define WIFI_DEINIT_DELAY_MS               500
#define WIFI_AFTER_CONNECT_FAIL_DELAY_MS  2000
#define WIFI_CONNECTION_TIMEOUT_MSW      30000


#define BUTTON_A_GPIO                  5
#define BUTTON_B_GPIO                  6
#define BUTTON_J_GPIO                 22

#define BUTTON_DEBONCE_MS             50

// I2C_MAIN used by all i2c sensors
#define I2C_MAIN_GPIO_SCL              1
#define I2C_MAIN_GPIO_SDA              0
#define I2C_MAIN_BAUDRATE         400000
#define I2C_MAIN_BUS                i2c0

// I2C_MAIN used by OLED
#define I2C_OLED_GPIO_SCL             15
#define I2C_OLED_GPIO_SDA             14
#define I2C_OLED_BAUDRATE         400000
#define I2C_OLED_BUS                i2c1

#define LED_R_GPIO                    13
#define LED_G_GPIO                    11
#define LED_B_GPIO                    12

#endif // CONFIG_H
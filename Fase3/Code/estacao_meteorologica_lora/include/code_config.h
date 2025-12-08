#ifndef CODE_CONFIG_H
#define CODE_CONFIG_H

#define NAME            "Weather Station"
#define VERSION         "0.00.00"
#define VERSION_DATA    "2025/12/08"
#define BUILD           "0003"


#define DEBUG_ON        true
#define DEBUG_ON_BAT    (DEBUG_ON || true)
#define DEBUG_ON_BMP280 (DEBUG_ON || true)

#define BOARD_BITDOG_LAB_V7
//#define BITDOGLAB_WITH_DEBUG_PROBE

#define HW_SLEEP_LOW_POWER false    //true


#define BAT_MAX_MV        4200     // 100%
#define BAT_MIN_MV        3300     //   0%

#define GPS_UART_ID         uart0
#define GPS_UART_BAUD_RATE  9600
#define GPS_UART_TX_PIN     0
#define GPS_UART_RX_PIN     1

#define GPIO_BUTTON_A        5
#define GPIO_BUTTON_B        6

#define GPIO_LED_RED        13
#define GPIO_LED_GREEN      11 
#define GPIO_LED_BLUE       12


#define LED_INIT_DELAY_MS   400


#define I2C_MAIN_GPIO_SCL              2
#define I2C_MAIN_GPIO_SDA              3
#define I2C_MAIN_BAUDRATE         400000
#define I2C_MAIN_BUS                i2c1


#endif // CODE_CONFIG_H
#ifndef CODE_CONFIG_H
#define CODE_CONFIG_H

#define NAME            "Weather Station"
#define VERSION         "0.00.01"
#define VERSION_DATA    "2026/01/24"
#define BUILD           "0019build"

#define CONFIG_DATA_VERSION         3   // uint32_t version;


#define DEBUG_ON                true
#define DEBUG_ON_BAT            (DEBUG_ON || true)
#define DEBUG_ON_BMP280         (DEBUG_ON || true)
#define DEBUG_ON_BMEP280        (DEBUG_ON || true)
#define DEBUG_ON_HW_SPEED       (DEBUG_ON || true)
//#define DEB_PR_CORE1_GPS        (DEBUG_ON || false)


#define BOARD_BITDOG_LAB_V7
//#define BOARD_BITDOG_LAB_V63
//#define BOARD_ESP32_C3

//#define BITDOGLAB_WITH_DEBUG_PROBE

#define HW_SLEEP_LOW_POWER true
#define ENABLE_LOOP_PRINTF
#define SEND_FIX_DATA

//#define ENABLE_GPIO_TEST
#define GPIO_TEST_0     18  // 8
#define GPIO_TEST_1     19  // 9


#define BAT_MAX_MV        4200     // 100%
#define BAT_MIN_MV        3300     //   0%
/*
#define GPS_UART_ID         uart0
#define GPS_UART_BAUD_RATE  9600
#define GPS_UART_TX_PIN     0
#define GPS_UART_RX_PIN     1
*/
#define GPS_UART_ID         uart1
#define GPS_UART_UART_IRQ   UART1_IRQ
#define GPS_UART_BAUD_RATE  9600
#define GPS_UART_TX_PIN     8
#define GPS_UART_RX_PIN     9

#define GPIO_BUTTON_A        5
#define GPIO_BUTTON_B        6

#define GPIO_LED_RED        13
#define GPIO_LED_GREEN      11 
#define GPIO_LED_BLUE       12

#define BMEP280_ADDRESS_0X77    false

#define INITIAL_DELAY_MS    10000    // para dar tempo de conectar a interface serial, não menos que 100
/*
#define WCM_UART_ID         uart1
#define WCM_UART_TX_PIN     8       // 0
#define WCM_UART_RX_PIN     9       // 1
#define WCM_UART_BAUD_RATE  115200
*/

#define WCM_UART_ID         uart0
#define WCM_UART_TX_PIN     0       // 0
#define WCM_UART_RX_PIN     1       // 1
#define WCM_UART_BAUD_RATE  115200


#define LED_INIT_DELAY_MS   400


#define I2C_MAIN_GPIO_SCL              2
#define I2C_MAIN_GPIO_SDA              3
#define I2C_MAIN_BAUDRATE         400000
#define I2C_MAIN_BUS                i2c1

#define I2C_TIMEOUT_US_BMEP280     10000


#endif // CODE_CONFIG_H
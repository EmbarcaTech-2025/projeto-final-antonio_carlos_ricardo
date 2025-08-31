# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.2.0)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.2.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico_w CACHE STRING "Board type")

set(CMAKE_BUILD_TYPE "Debug")
set(PICO_DEOPTIMIZED_DEBUG 1)


# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(estacao_meteorologica_01 C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(estacao_meteorologica_01
        main.c
        lib/oled_driver/ssd1306_i2c.c
        lib/bmp280/bmp280.c
        lib/bmp280/bmp280_low_level.c
        lib/src/aht10.c
        lib/src/bh1750.c
        lib/oled/display.c
        lib/oled/i2c.c
        lib/src/rtc_ds3231.c
        src/ad_board.c
        src/config_data.c
        src/core_communication.c
        src/core0_comm.c
        src/core1_acquisition.c
        src/core1_data.c
        src/core1_main.c
        src/estation_data.c
        src/led_rgb.c
        src/menu_config.c
        src/menu_rtc.c
        src/radio.c
        src/radio_common.c
        src/radio_get_time.c
        src/radio_send_data.c
        src/rtc_wrap.c
        src/storage.c
        src/storage_adm.c
        src/testes.c
        src/ui_display.c
        src/util.c
        src/wifi.c
        )

pico_set_program_name(estacao_meteorologica_01 "estacao_meteorologica_01")
pico_set_program_version(estacao_meteorologica_01 "0.1")

# no_flash means the target is to run from RAM
# pico_set_binary_type(estacao_meteorologica_01 no_flash)

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(estacao_meteorologica_01 0)
pico_enable_stdio_usb(estacao_meteorologica_01 1)

# Add the standard library to the build
target_link_libraries(estacao_meteorologica_01
        pico_multicore
        pico_stdlib
        hardware_adc
        hardware_flash
        hardware_i2c
        # Oferece suporte Wi-Fi (CYW43) com LwIP (pilha TCP/IP) em um modo thread-safe em segundo plano.
        pico_cyw43_arch_lwip_threadsafe_background
        # O driver do chip Wi-Fi CYW43 (usado no Pico W).
#        pico_cyw43_driver
        # A pilha Lightweight IP (LwIP), que fornece funcionalidades TCP/IP (HTTP, MQTT, UDP, DHCP, etc.).
#        pico_lwip
        )

# Add the standard include files to the build
target_include_directories(estacao_meteorologica_01 PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

# Add any user requested libraries
target_link_libraries(estacao_meteorologica_01 
        
        )

pico_add_extra_outputs(estacao_meteorologica_01)


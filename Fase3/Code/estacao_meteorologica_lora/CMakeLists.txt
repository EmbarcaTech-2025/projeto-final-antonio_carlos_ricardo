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
set(picotoolVersion 2.2.0-a4)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico CACHE STRING "Board type")

# set(CMAKE_BUILD_TYPE "Debug")
# set(PICO_DEOPTIMIZED_DEBUG 1)

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(estacao_meteorologica_lora C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(estacao_meteorologica_lora
        main.c
        src/aq_data_bat.c
        src/aq_data_bme280.c
        src/aq_data_cpu_temp.c
        src/aq_data_gps.c
        src/aq_data_lux.c
        src/aq_data.c
        src/bh1750.c
        src/bmp280/bmp280.c
        src/bmp280/bmp280_low_level.c
        src/buttons_and_leds.c
        src/gps/gy_neo6mv2.c
        src/est_config.c
        src/hw_sleep.c
        src/menu_conf.c
        src/wcm.c
        src/storage.c
        )

pico_set_program_name(estacao_meteorologica_lora "estacao_meteorologica_lora")
pico_set_program_version(estacao_meteorologica_lora "0.1")

# no_flash means the target is to run from RAM
# pico_set_binary_type(estacao_meteorologica_lora no_flash)

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(estacao_meteorologica_lora 0)
pico_enable_stdio_usb(estacao_meteorologica_lora 1)

# Add the standard library to the build
target_link_libraries(estacao_meteorologica_lora
        pico_stdlib
        hardware_adc
        hardware_i2c
        )

# Add the standard include files to the build
target_include_directories(estacao_meteorologica_lora PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

pico_add_extra_outputs(estacao_meteorologica_lora)


#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "../include/bh1750.h"
#include "../include/config.h"

#define BH1750_ADDR 0x23 // ou 0x5C, dependendo do sensor
#define BH1750_CONT_HRES_MODE 0x10 // Modo contínuo, alta resolução

/*
--- INICIALIZAR SENSOR BH ---
    Inicializa o BH1750 em modo contínuo de medição de alta resolução.
*/
void bh1750_init() {
    uint8_t cmd = BH1750_CONT_HRES_MODE;
    i2c_write_blocking(I2C_MAIN_BUS, BH1750_ADDR, &cmd, 1, false);
}

/*
--- LEITURA DO SENSOR BH1750 ---
    Lê 2 bytes da medição de iluminância (16 bits) diretamente do
    sensor, e converte o valor bruto para lux, usando fator de escala
    padrão (1.2).
*/
uint16_t bh1750_read_lux() {
    uint8_t data[2];
    i2c_read_blocking(I2C_MAIN_BUS, BH1750_ADDR, data, 2, false);
    uint16_t raw = (data[0] << 8) | data[1];
    // Conversão: 1 lux = raw / 1.2
    return raw / 1.2;
}
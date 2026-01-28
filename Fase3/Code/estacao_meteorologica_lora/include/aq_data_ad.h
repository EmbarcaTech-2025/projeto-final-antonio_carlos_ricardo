#ifndef AQ_DATA_AD_H
#define AQ_DATA_AD_H

#include "pico/stdlib.h"

int aqdataad_init_power_on(bool vsys, bool temp, uint16_t vsys_k_10000);
int aqdataad_init_aq(bool vsys, bool temp);
int aqdataad_read_temp(int16_t *value);
int aqdataad_read_vsys(uint8_t *value);
int aqdataad_sleep(bool vsys, bool temp);


#endif // AQ_DATA_AD_H
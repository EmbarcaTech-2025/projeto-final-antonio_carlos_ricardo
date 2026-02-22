#ifndef BMP280_MAIN_H
#define BMP280_MAIN_H

/*typedef struct
{
  float temperature;
  uint32_t pressure;
  float altitude;
} sensors_t;*/

typedef struct
{
  float temperature;
  uint32_t pressure;
  float altitude;
} sensors_t;

void bmp280_main_init();

sensors_t bmp280_main_get_all_0();

#endif // BMP280_MAIN_H
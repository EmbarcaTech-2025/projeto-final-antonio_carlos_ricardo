#ifndef ESTATION_DATA_H
#define ESTATION_DATA_H

typedef struct{
    uint64_t cycle; // começa com 1
    uint64_t t_aquire;
    uint64_t t_send;

    //I2c
    int32_t  press;
    int32_t  temp_p;
    int32_t  humidity;
    int32_t  temp_h;
    int32_t  wind_dir;
    int32_t  luminos;

    //Digital
    int32_t  wind_speed;
    int32_t  rain_intensity;

    //Analog
    int32_t  analog_ar_co;
    int32_t  analog_ar_co2;
    int32_t  analog_ar_o3;
    int32_t  soil_humidity;
    int32_t  soil_temperature;
    int32_t  uv_index;
}EstationData;

void     estation_data_clear(EstationData * estation_data);

int16_t estation_data_json(EstationData * estation_data, char * buf);

extern char estation_data_buf_tmp[];







#endif // ESTATION_DATA_H
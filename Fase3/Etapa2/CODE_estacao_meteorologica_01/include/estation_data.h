/**
 * @file    estation_data.h
 * @author  Carlos e Ricardo
 * @brief   Define a estrutura de dados da estação, e
 * - limpa estes dados
 * - converte para string
 * - gera string faike
 * - disponibiliza bugger temporário para gerar o string
 * @version 0.1
 * @date    2025-09-16
 * 
 * @copyright Copyright (c) 2025
 */
#ifndef ESTATION_DATA_H
#define ESTATION_DATA_H

/**
 * @brief Estrutura dos dados da estação
 * 
 */
typedef struct{
    uint64_t cycle; // começa com 1, 0 ==> bloco com problemas, 0xFFFFFFFFFFFFFFFF => bloco livre
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

/**
 * @brief bugger temporário para gerar o string
 * 
 */
extern char estation_data_buf_tmp[];

/**
 * @brief Limpa os dados da estação com 0xFF
 * 
 * @param estation_data 
 */
void     estation_data_clear(EstationData * estation_data);

/**
 * @brief Converte os dados da estação em um string JSON
 * 
 * @param estation_data 
 * @param buf 
 * @return int16_t 
 */
int16_t estation_data_json(EstationData * estation_data, char * buf);

/**
 * @brief Escreve no buffer um json com todos os dados, mas os dados são tais que refletem um dos maiores strings possíveis
 * 
 * @param buf, a onde são escritos os registros 
 * @param aq_n, número da aquisição, para diferenciar um registro de outro 
 * @return int16_t, retorna quantos bytes foram utilizados
 */
int16_t estation_data_json_full_fake(char * buf, uint64_t aq_n);

/**
 * @brief Imprime todos os dados que estão na flash
 * - util para debug
 */
void    estation_data_printf();




#endif // ESTATION_DATA_H
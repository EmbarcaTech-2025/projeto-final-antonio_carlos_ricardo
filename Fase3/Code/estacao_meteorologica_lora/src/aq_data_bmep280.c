#include <stdio.h>
#include "hardware/i2c.h"
#include "../include/aq_data_bmep280.h"
#include "../include/code_config.h"


#include "../include/loop_printf.h"


#define AQDATABMEP280_CHIP_ID_BMP180    0x55
#define AQDATABMEP280_CHIP_ID_BMP280    0x58
#define AQDATABMEP280_CHIP_ID_BME280    0x60
#define AQDATABMEP280_CHIP_ID_BME680    0x61


// Definições usadas na rotina
#define BME280_S32_t        int32_t
#define BME280_U32_t        uint32_t
#define BME280_S64_t        int64_t

typedef struct {        // pag24
    uint16_t dig_T1;
    int16_t  dig_T2;
    int16_t  dig_T3;

    uint16_t dig_P1;
    int16_t  dig_P2;
    int16_t  dig_P3;
    int16_t  dig_P4;
    int16_t  dig_P5;
    int16_t  dig_P6;
    int16_t  dig_P7;
    int16_t  dig_P8;
    int16_t  dig_P9;

    // Only BME280
    uint8_t  not_used;
    uint8_t  dig_H1;
    int16_t  dig_H2;
    uint8_t  dig_H3;
    int16_t  dig_H4;
    int16_t  dig_H5;
    int8_t   dig_H6;
} AqDataBmep280_Calibration;


typedef struct {
    uint32_t temp;
    uint32_t press;
    uint16_t hum;       // Only BME280
} AqDataBmep280_RawValues;



static AqDataBmep280_Calibration calibration;
static AqDataBmep280_RawValues   raw_values;

static AqDataBmep280_Device      device_id          = AQDATABMEP280_DEV_ERROR;
static uint8_t                   device_i2c_address = 0x76;   // ou 0x77






static int bmp280_write_byte(uint8_t start_register, uint8_t data){
    uint8_t d[] = {start_register, data};

    if (i2c_write_timeout_us(I2C_MAIN_BUS, device_i2c_address, d, 2, false, I2C_TIMEOUT_US_BMEP280) != 1) return -1;
    return 0;
}

static int registers_read(uint8_t start_register, uint8_t *data, uint8_t data_size){
    // Escreve o endereço do start_register
    if (i2c_write_timeout_us(I2C_MAIN_BUS, device_i2c_address, &start_register, 1, true, I2C_TIMEOUT_US_BMEP280) != 1) return -1;
    
    // Lê os dados
    if(i2c_read_timeout_us(I2C_MAIN_BUS, device_i2c_address, data, data_size, false, I2C_TIMEOUT_US_BMEP280) != data_size) return -2;

    return 0;
}





static int aqdatabmep280_read_device(){
    uint8_t data;

    device_id = AQDATABMEP280_DEV_ERROR;
    int ret = registers_read(0xD0, &data, 1);
    printf("read_device(): AD=0x%02x, data=0x%02x, ret=%d  %s\n", device_i2c_address, data, ret, 
        data==AQDATABMEP280_CHIP_ID_BMP280?"bmP280":(data==AQDATABMEP280_CHIP_ID_BME280?"bmE280":"ERROR"));
    if(ret) return ret;
    switch(data){
        case AQDATABMEP280_CHIP_ID_BMP280: device_id = AQDATABMEP280_DEV_BMP280; break;
        case AQDATABMEP280_CHIP_ID_BME280: device_id = AQDATABMEP280_DEV_BME280; break;
        default: return -1000;
    }
    return 0;
}
static int aqdatabmep280_read_calibration(){

    for(int i=0;i<sizeof(calibration);i++){
        ((uint8_t *)&calibration)[i] = 0xFF;
    }

    
    uint8_t n_itens;

    switch(device_id){
        case AQDATABMEP280_DEV_BMP280: n_itens = 24; break;
        case AQDATABMEP280_DEV_BME280: n_itens = 26; break;
        default: return -1000;
    }
    printf("read cal 1\n");

    int ret = registers_read(0x88, (uint8_t *)&calibration, n_itens);
    if(ret) return ret;

    printf("read cal 2\n");

    uint8_t datad[26];
    int retd = registers_read(0x88, datad, 26);
    for(int i=0;i<26;i++) printf("%02x ", datad[i]);
    printf("\n0x%04x  %04x  %04x  %04x  %04x  %04x  %02x\n", calibration.dig_T1, calibration.dig_T2,calibration.dig_T3, calibration.dig_P1, calibration.dig_P2, calibration.dig_P3, calibration.dig_H1);

    if(device_id != AQDATABMEP280_DEV_BME280) return 0;


    uint8_t data[7];
    int ret2 = registers_read(0xE1, data, 7);

/*
    data[0] = 0x01;
    data[1] = 0x23;
    data[2] = 0x45;
    data[3] = 0x67;
    data[4] = 0x89;
    data[5] = 0xAB;
    data[6] = 0xCD;
    data[7] = 0xEF;
*/

    calibration.dig_H2 =  (data[0]&0x00FF)       | ((data[1]&0x00FF)<<8);
    calibration.dig_H3 =   data[2];
    calibration.dig_H4 =  (data[4]&0x000F)       | ((data[3]&0x00FF)<<4);
    calibration.dig_H5 = ((data[4]&0x00F0) >> 4) | ((data[5]&0x00FF)<<4);
    calibration.dig_H6 =   data[6];

    for(int i=0;i<7;i++) printf("%02x ", data[i]);
    printf("\n0x%04x  %02x  %04x  %04x  %02x\n", calibration.dig_H2, calibration.dig_H3,calibration.dig_H4, calibration.dig_H5, calibration.dig_H6);

    return 0;
}

static int aqdatabmep280_wait_aquisition(){

}
static int aqdatabmep280_read_raw_values(){
    uint8_t data[8];
    int ret;

    // aguarda fim da conversão
    bool fim = false;
    while(!fim){
        ret = registers_read(0xF3, data, 1);
        if(!(data[0] & 0x08)) fim = true;
    }

    ret = registers_read(0xF7, data, 8);
    if(ret) return ret;

    raw_values.press = data[0];
    raw_values.press = raw_values.press << 8;
    raw_values.press |= data[1];
    raw_values.press = raw_values.press << 4;
    raw_values.press |= data[2] >> 4;

    raw_values.temp = data[3];
    raw_values.temp = raw_values.temp << 8;
    raw_values.temp |= data[4];
    raw_values.temp = raw_values.temp << 4;
    raw_values.temp |= data[5] >> 4;

    raw_values.hum = data[6];
    raw_values.hum = raw_values.hum << 8;
    raw_values.hum |= data[7];

    for(int i=0;i<8;i++) printf("%02x ", data[i]);
    printf("\nRaw: temp=0x%08x, press=0x%08x, hum=0x%04x\n", raw_values.temp, raw_values.press, raw_values.hum);

    return 0;
};


static int aqdatabmep280_read_raw_values0(){
    uint8_t data[8];
    int ret;

    // habilita umidade
    ret =  bmp280_write_byte(0xF2, 0x07);

    // inicia conversão:
    ret =  bmp280_write_byte(0xF4, 0xFE);


    return aqdatabmep280_read_raw_values();

};




int aqdatabmep280_init_power_on(){
    device_i2c_address = BMEP280_ADDRESS_0X77? 0x77:0x76;
    if(aqdatabmep280_read_device()) return 1;
    return aqdatabmep280_read_calibration();
}
int aqdatabmep280_init_aq(){
    int ret;

    // habilita umidade
    ret =  bmp280_write_byte(0xF2, 0x07);
    // inicia conversão:
    ret +=  bmp280_write_byte(0xF4, 0xFE) * 10;

    return ret;
}


BME280_S32_t BME280_compensate_T_int32(BME280_S32_t adc_T);
BME280_U32_t BME280_compensate_P_int64(BME280_S32_t adc_P);
BME280_U32_t bme280_compensate_H_int32(BME280_S32_t adc_H);





int aqdatabmep280_read_i(AqDataBmep280_Value_I *value){
    aqdatabmep280_read_raw_values();
 
    int32_t  t = BME280_compensate_T_int32(raw_values.temp);     // 1/100 Celsisus
    uint32_t p = BME280_compensate_P_int64( raw_values.press);   // 1/256 hPa

    uint32_t u = 0xFF;

    if(device_id == AQDATABMEP280_DEV_BME280){
        u = bme280_compensate_H_int32( raw_values.hum);     // 1/1024 %
        u = u / 512;
        if(u>200) u = 200;
    }

    if(DEBUG_ON_BMEP280){
        loop_printf("BMP280-Temp     = %.2f Celsius\n", t * 0.01);
        loop_printf("BMP280-press    = %.2f hPa\n",     p / 256.0);
        loop_printf("BMP280-Humi     = %.1f %%\n",      u * 0.5);
    }

    value->humidity = u;     // resolução 0,5%
    value->temp     = t;
    value->press    = 60000 - (p * 100 / 256 / 2);    //resolução 0,02 hpa

    return 0;
}
int aqdatabmep280_sleep(){
    return 0;
}







// ********** Necessarios para as rotinas do BME280 datasheet **********




// ********** From BME280 datasheet pag. 25 **********

BME280_S32_t t_fine;    // t_fine carries fine temperature as global value

// Returns temperature in DegC, resolution is 0.01 DegC. Output value of “5123” equals 51.23 DegC.
BME280_S32_t BME280_compensate_T_int32(BME280_S32_t adc_T)
{
    BME280_S32_t var1, var2, T;
    var1 = ((((adc_T>>3) - ((BME280_S32_t)calibration.dig_T1<<1))) * ((BME280_S32_t)calibration.dig_T2)) >> 11;
    var2 = (((((adc_T>>4) - ((BME280_S32_t)calibration.dig_T1)) * ((adc_T>>4) - ((BME280_S32_t)calibration.dig_T1))) >> 12) * ((BME280_S32_t)calibration.dig_T3)) >> 14;
    t_fine = var1 + var2;
    T = (t_fine * 5 + 128) >> 8;
    return T;
}
// Returns pressure in Pa as unsigned 32 bit integer in Q24.8 format (24 integer bits and 8 fractional bits).
// Output value of “24674867” represents 24674867/256 = 96386.2 Pa = 963.862 hPa
BME280_U32_t BME280_compensate_P_int64(BME280_S32_t adc_P)
{
    BME280_S64_t var1, var2, p;
    var1 = ((BME280_S64_t)t_fine) - 128000;
    var2 = var1 * var1 * (BME280_S64_t)calibration.dig_P6;
    var2 = var2 + ((var1*(BME280_S64_t)calibration.dig_P5)<<17);
    var2 = var2 + (((BME280_S64_t)calibration.dig_P4)<<35);
    var1 = ((var1 * var1 * (BME280_S64_t)calibration.dig_P3)>>8) + ((var1 * (BME280_S64_t)calibration.dig_P2)<<12);
    var1 = (((((BME280_S64_t)1)<<47)+var1))*((BME280_S64_t)calibration.dig_P1)>>33;
    if (var1 == 0)
    {
        return 0; // avoid exception caused by division by zero
    }
    p = 1048576-adc_P;
    p = (((p<<31)-var2)*3125)/var1;
    var1 = (((BME280_S64_t)calibration.dig_P9) * (p>>13) * (p>>13)) >> 25;
    var2 = (((BME280_S64_t)calibration.dig_P8) * p) >> 19;
    p = ((p + var1 + var2) >> 8) + (((BME280_S64_t)calibration.dig_P7)<<4);
    return (BME280_U32_t)p;
}
// Returns humidity in %RH as unsigned 32 bit integer in Q22.10 format (22 integer and 10 fractional bits).
// Output value of “47445” represents 47445/1024 = 46.333 %RH
BME280_U32_t bme280_compensate_H_int32(BME280_S32_t adc_H)
{
    BME280_S32_t v_x1_u32r;
    v_x1_u32r = (t_fine - ((BME280_S32_t)76800));
    v_x1_u32r = (((((adc_H << 14) - (((BME280_S32_t)calibration.dig_H4) << 20) - (((BME280_S32_t)calibration.dig_H5) *
        v_x1_u32r)) + ((BME280_S32_t)16384)) >> 15) * (((((((v_x1_u32r *
        ((BME280_S32_t)calibration.dig_H6)) >> 10) * (((v_x1_u32r * ((BME280_S32_t)calibration.dig_H3)) >> 11) +
        ((BME280_S32_t)32768))) >> 10) + ((BME280_S32_t)2097152)) * ((BME280_S32_t)calibration.dig_H2) +
        8192) >> 14));
    v_x1_u32r = (v_x1_u32r - (((((v_x1_u32r >> 15) * (v_x1_u32r >> 15)) >> 7) *
        ((BME280_S32_t)calibration.dig_H1)) >> 4));
    v_x1_u32r = (v_x1_u32r < 0 ? 0 : v_x1_u32r);
    v_x1_u32r = (v_x1_u32r > 419430400 ? 419430400 : v_x1_u32r);
    return (BME280_U32_t)(v_x1_u32r>>12);
}

// forced mode

// ********** From BME280 datasheet pag. 49 **********

// Returns temperature in DegC, double precision. Output value of “51.23” equals 51.23 DegC.
// t_fine carries fine temperature as global value
BME280_S32_t t_fine;
double BME280_compensate_T_double(BME280_S32_t adc_T)
{
    double var1, var2, T;
    var1 = (((double)adc_T)/16384.0 - ((double)calibration.dig_T1)/1024.0) * ((double)calibration.dig_T2);
    var2 = ((((double)adc_T)/131072.0 - ((double)calibration.dig_T1)/8192.0) *
        (((double)adc_T)/131072.0 - ((double)calibration.dig_T1)/8192.0)) * ((double)calibration.dig_T3);
    t_fine = (BME280_S32_t)(var1 + var2);
    T = (var1 + var2) / 5120.0;
    return T;
}
// Returns pressure in Pa as double. Output value of “96386.2” equals 96386.2 Pa = 963.862 hPa
double BME280_compensate_P_double(BME280_S32_t adc_P)
{
    double var1, var2, p;
    var1 = ((double)t_fine/2.0) - 64000.0;
    var2 = var1 * var1 * ((double)calibration.dig_P6) / 32768.0;
    var2 = var2 + var1 * ((double)calibration.dig_P5) * 2.0;
    var2 = (var2/4.0)+(((double)calibration.dig_P4) * 65536.0);
    var1 = (((double)calibration.dig_P3) * var1 * var1 / 524288.0 + ((double)calibration.dig_P2) * var1) / 524288.0;
    var1 = (1.0 + var1 / 32768.0)*((double)calibration.dig_P1);
    if (var1 == 0.0)
    {
        return 0; // avoid exception caused by division by zero
    }
    p = 1048576.0 - (double)adc_P;
    p = (p - (var2 / 4096.0)) * 6250.0 / var1;
    var1 = ((double)calibration.dig_P9) * p * p / 2147483648.0;
    var2 = p * ((double)calibration.dig_P8) / 32768.0;
    p = p + (var1 + var2 + ((double)calibration.dig_P7)) / 16.0;
    return p;
}
// Returns humidity in %rH as as double. Output value of “46.332” represents 46.332 %rH
double bme280_compensate_H_double(BME280_S32_t adc_H)
{
    double var_H;
    var_H = (((double)t_fine) - 76800.0);
    var_H = (adc_H - (((double)calibration.dig_H4) * 64.0 + ((double)calibration.dig_H5) / 16384.0 *
        var_H)) * (((double)calibration.dig_H2) / 65536.0 * (1.0 + ((double)calibration.dig_H6) /
        67108864.0 * var_H *
        (1.0 + ((double)calibration.dig_H3) / 67108864.0 * var_H)));
    var_H = var_H * (1.0 - ((double)calibration.dig_H1) * var_H / 524288.0);
    if (var_H > 100.0)
        var_H = 100.0;
    else if (var_H < 0.0)
    var_H = 0.0;

    return var_H;
}





// -------------------------------------------------------------------------






static void teste01(uint8_t da, char * s){
    uint8_t data;

    uint8_t data2[26];
/*
    Ud ud;

    device_i2c_address = da;
    int ret1 = registers_read(0xD0, &data, 1);
    int ret2 = registers_read(0x88, data2, 26);
    int ret3 = registers_read(0x88, ud.data,   35);

    printf("%s) AD=0x%02x, data=0x%02x, ret1=%d, ret2=%d, ret3=%d\n", s, device_i2c_address, data, ret1, ret2, ret3);

    for(int i=0;i<26;i++){
        printf("%02x ", data2[i]);
    }
    printf("\n");
    for(int i=0;i<36;i++){
        printf("%02x ", ud.data[i]);
    }
    printf("\n0x%04x  %04x  %04x  %04x  %04x  %04x  %-2x", ud.cv.dig_T1, ud.cv.dig_T2, ud.cv.dig_T3, ud.cv.dig_P1, ud.cv.dig_P2, ud.cv.dig_P3, ud.cv.dig_H1);
    printf("\n\n");

  */  


}

void aqdatabmep280_teste(){
/*

    bmp280_main_init();
    sensors_t bmp280 = bmp280_main_get_all_0();
    
    loop_printf("BMP280-Altitude = %f meters\n", bmp280.altitude);
    loop_printf("BMP280-Temp     = %f \n",       bmp280.temperature);
    loop_printf("BMP280-press    = %d \n",       bmp280.pressure);
  */  
    int count = 0;
    uint8_t data;
    int ret;

    double  humf;
    int32_t humi;
    double  pressf;
    int32_t pressi;
    double  tempf;
    int32_t tempi;

    while(1){
        printf("bmE 280\n");
        device_i2c_address = 0x76;
        aqdatabmep280_read_device();
        aqdatabmep280_read_calibration();
        aqdatabmep280_read_raw_values();


        tempf = BME280_compensate_T_double(raw_values.temp);
        tempi = BME280_compensate_T_int32(raw_values.temp);

        pressf = BME280_compensate_P_double(raw_values.press);
        pressi = BME280_compensate_P_int64( raw_values.press);

        humf = bme280_compensate_H_double(raw_values.hum);
        humi = bme280_compensate_H_int32( raw_values.hum);

        printf("Temp=%f  %d, Press==%f  %d, Hum=%f  %d\n", tempf, tempi, pressf, pressi, humf, humi);
        sleep_ms(200);

        printf("bmP 280\n");
        device_i2c_address = 0x77;
        aqdatabmep280_read_device();
        aqdatabmep280_read_calibration();
        aqdatabmep280_read_raw_values();

        tempf = BME280_compensate_T_double(raw_values.temp);
        tempi = BME280_compensate_T_int32( raw_values.temp);

        pressf = BME280_compensate_P_double(raw_values.press);
        pressi = BME280_compensate_P_int64( raw_values.press);

        humf = bme280_compensate_H_double(raw_values.hum);
        humi = bme280_compensate_H_int32( raw_values.hum);

        printf("Temp=%f  %d, Press==%f  %d\n", tempf, tempi, pressf, pressi);
        sleep_ms(200);

        printf("\n\n");
    }

}
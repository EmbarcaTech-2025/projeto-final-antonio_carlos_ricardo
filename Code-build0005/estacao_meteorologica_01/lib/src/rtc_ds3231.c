
#include <stdio.h>
#include <time.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "../../include/config.h"
#include "../include/rtc_ds3231.h"


#define I2C_ADDR_DS3231 0x68

// --- Conversão BCD ---
static uint8_t bcd_to_dec(uint8_t val) {
    return (val / 16 * 10) + (val % 16);
}
static uint8_t dec_to_bcd(uint8_t val) {
    return (val / 10 * 16) + (val % 10);
}


uint64_t rtc_ds3231_gerar_timestamp(int ano, int mes, int dia, int hora, int minuto, int segundo) {
    struct tm t = {0};

    t.tm_year = ano - 1900; // struct tm conta anos desde 1900
    t.tm_mon  = mes - 1;    // struct tm conta meses de 0 a 11
    t.tm_mday = dia;
    t.tm_hour = hora;
    t.tm_min  = minuto;
    t.tm_sec  = segundo;
    t.tm_isdst = 0; // Sem horário de verão

    return (uint64_t)mktime(&t);
}
void rtc_ds3231_timestamp_to_string(uint64_t timestamp, char * str){
    // str size 24
    struct tm t;
    gmtime_r((time_t*)&timestamp, &t);
    sprintf(str, "%04d-%02d-%02d %02d:%02d'%02d\" %d",
        t.tm_year - 100 + 2000,
        t.tm_mon + 1,
        t.tm_mday,
        t.tm_hour,
        t.tm_min,
        t.tm_sec,
        t.tm_wday   // 0 ==> domingo
    );
}


// --- Escrever data/hora no DS3231 ---
void rtc_ds3231_set(uint64_t timestamp) {
    struct tm t;
    gmtime_r((time_t*)&timestamp, &t);

    uint8_t buf[8];
    buf[0] = 0;
    buf[1] = dec_to_bcd(t.tm_sec);
    buf[2] = dec_to_bcd(t.tm_min);
    buf[3] = dec_to_bcd(t.tm_hour);
    buf[4] = dec_to_bcd(t.tm_wday + 1); // DS3231 usa 1=Domingo
    buf[5] = dec_to_bcd(t.tm_mday);
    buf[6] = dec_to_bcd(t.tm_mon + 1);
    buf[7] = dec_to_bcd(t.tm_year - 100); // desde 2000

    // quando se vai escrever o primeiro byte é o endereço, não pode ter outro start
    i2c_write_blocking(I2C_MAIN_BUS, I2C_ADDR_DS3231, buf, 8, false);
}

uint64_t rtc_ds3231_get() {
    uint8_t buf[7];
    uint8_t reg = 0x00;
    i2c_write_blocking(I2C_MAIN_BUS, I2C_ADDR_DS3231, &reg, 1, true);
    i2c_read_blocking( I2C_MAIN_BUS, I2C_ADDR_DS3231, buf,  7, false);

    struct tm t;
    t.tm_sec  = bcd_to_dec(buf[0]);
    t.tm_min  = bcd_to_dec(buf[1]);
    t.tm_hour = bcd_to_dec(buf[2]);
    t.tm_wday = bcd_to_dec(buf[3]) - 1;
    t.tm_mday = bcd_to_dec(buf[4]);
    t.tm_mon  = bcd_to_dec(buf[5]) - 1;
    t.tm_year = bcd_to_dec(buf[6]) + 100;
;
    t.tm_isdst = 0;
    return mktime(&t);
}


void rtc_ds3231_test(){
    char buf[32];
    uint64_t timestamp;

    printf("---------- rtc_ds3231_test ----------\n\n");

    timestamp =rtc_ds3231_get();
    rtc_ds3231_timestamp_to_string(timestamp, buf);
    printf("Inicial ==> %s   %llu\n\n", buf, timestamp);

    timestamp = 0;
    rtc_ds3231_timestamp_to_string(timestamp, buf);
    printf("timestamp = 0 ==> %s\n", buf);

    //                                      ano,mes,dia,hora, min, seg
    timestamp = rtc_ds3231_gerar_timestamp(1970,  1,  1,   0,  0,  0);
    rtc_ds3231_timestamp_to_string(timestamp, buf);
    printf("1970,  1,  1,   0,  0,  0 ==> %s   %llu\n", buf, timestamp);
    rtc_ds3231_set(timestamp);
    timestamp =rtc_ds3231_get();
    rtc_ds3231_timestamp_to_string(timestamp, buf);
    printf("1970,  1,  1,   0,  0,  0 ==> %s   %llu\n\n", buf, timestamp);

    timestamp = rtc_ds3231_gerar_timestamp(1970,  1,  3,   0,  0,  0);
    rtc_ds3231_timestamp_to_string(timestamp, buf);
    printf("1970,  1,  3,   0,  0,  0 ==> %s   %llu\n", buf, timestamp);
    rtc_ds3231_set(timestamp);
    timestamp =rtc_ds3231_get();
    rtc_ds3231_timestamp_to_string(timestamp, buf);
    printf("1970,  1,  3,   0,  0,  0 ==> %s   %llu\n\n", buf, timestamp);

    timestamp = rtc_ds3231_gerar_timestamp(2000,  1,  1,   0,  0,  0);
    rtc_ds3231_timestamp_to_string(timestamp, buf);
    printf("2000,  1,  1,   0,  0,  0 ==> %s   %llu\n", buf, timestamp);
    rtc_ds3231_set(timestamp);
    timestamp =rtc_ds3231_get();
    rtc_ds3231_timestamp_to_string(timestamp, buf);
    printf("2000,  1,  1,   0,  0,  0 ==> %s   %llu\n\n", buf, timestamp);

    timestamp = rtc_ds3231_gerar_timestamp(2000,  2,  3,   0,  0,  0);
    rtc_ds3231_timestamp_to_string(timestamp, buf);
    printf("2000,  2,  3,   0,  0,  0 ==> %s   %llu\n", buf, timestamp);
    rtc_ds3231_set(timestamp);
    timestamp =rtc_ds3231_get();
    rtc_ds3231_timestamp_to_string(timestamp, buf);
    printf("2000,  2,  3,   0,  0,  0 ==> %s   %llu\n\n", buf, timestamp);

    timestamp = rtc_ds3231_gerar_timestamp(2010,  1,  1,   0,  0,  0);
    rtc_ds3231_timestamp_to_string(timestamp, buf);
    printf("2010,  1,  1,   0,  0,  0 ==> %s   %llu\n", buf, timestamp);
    rtc_ds3231_set(timestamp);
    timestamp =rtc_ds3231_get();
    rtc_ds3231_timestamp_to_string(timestamp, buf);
    printf("2010,  1,  1,   0,  0,  0 ==> %s   %llu\n\n", buf, timestamp);

    timestamp = rtc_ds3231_gerar_timestamp(2025,  8, 22,  14, 31, 26);
    rtc_ds3231_timestamp_to_string(timestamp, buf);
    printf("2025,  8, 22,  14, 31, 26 ==> %s   %llu\n", buf, timestamp);
    rtc_ds3231_set(timestamp);
    timestamp =rtc_ds3231_get();
    rtc_ds3231_timestamp_to_string(timestamp, buf);
    printf("2025,  8, 22,  14, 31, 26 ==> %s   %llu\n\n", buf, timestamp);
    
    while(1){
        sleep_ms(1);
    }
}




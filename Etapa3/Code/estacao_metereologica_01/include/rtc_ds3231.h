#ifndef RTC_DS3231_H
#define RTC_DS3231_H


uint64_t rtc_ds3231_gerar_timestamp(int ano, int mes, int dia, int hora, int minuto, int segundo);
void     rtc_ds3231_timestamp_to_string(uint64_t timestamp, char * str);
uint64_t rtc_ds3231_get();
void     rtc_ds3231_set(uint64_t timestamp);
void     rtc_ds3231_test();


#endif // RTC_DS3231_H
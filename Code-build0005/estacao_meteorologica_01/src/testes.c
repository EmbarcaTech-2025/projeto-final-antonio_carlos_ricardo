#include <pico/cyw43_arch.h>
#include "../include/testes.h"
#include "../lib/include/aht10.h"
#include "../lib/include/bmp280_main.h"
#include "../lib/include/bh1750.h"
#include "../lib/include/rtc_ds3231.h"


void testes_i2c(){
    aht10_init();
    while(1){
        Aht10Data d = aht10_read();
        printf("Temp = %.2f    Humi==%.2f\n", d.temp, d.humidity);

        sleep_ms(100);
    }


    bh1750_init();
    while(1){
        int16_t l = bh1750_read_lux();
        printf("luminosidade = %lu lux\n", l);
    }



    bmp280_main_init();
    while(1){
        sensors_t s;
        s = bmp280_main_get_all_0();
        printf("Temp=%.2f  press=%d  alt=%.1f\n", s.temperature, s.pressure, s.altitude);
        sleep_ms(500);
    }

    rtc_ds3231_test();
    
}


/*
            bool ret = radio_send_est_data(estation_data_buf_tmp);
            printf("Core0-SHOW-Data_transmite teste ret=%s\n", ret?"true":"false");

            if(DEB_PR_CORE0_SHOW){
                printf("Core0-SHOW-Data size = %d\n", size);
                printf("Core0-SHOW-Data: ");
                printf(estation_data_buf_tmp);
                printf("\n");
                
                // exemplo de maior mensagem, atualmente sise = 408
                size = estation_data_json_full_fake(&core_comm_estation_data, estation_data_buf_tmp, 0xFFFFFFFFFFFFFFFF);
                printf("Core0-SHOW-DummyFull-Data size = %d\n", size);
                printf("Core0-SHOW-DummyFull-Data: ");
                printf(estation_data_buf_tmp);
                printf("\n");
                
            }



            int16_t size;

            printf("Core0-SHOW-Request time\n");
            uint64_t rt;
            bool ret = radio_get_time_r(&rt);
            printf("Core0-SHOW-Request time, ret=%s, v=%llu\n", ret?"true":"false", rt);

            struct tm tm_utc;
            gmtime_r(&rt, &tm_utc);
            char buf[32];
            strftime(buf, 31, "%Y-%m-%dT%H:%M:%SZ", &tm_utc);
            printf("Core0-SHOW-Request time, time=%s\n", buf);


            size = estation_data_json(&core_comm_estation_data, estation_data_buf_tmp);
            */

            // ui_display_update(&core_comm_estation_data);
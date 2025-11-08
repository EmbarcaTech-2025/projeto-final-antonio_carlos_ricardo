#include <time.h>
#include <pico/cyw43_arch.h>
#include "../include/util.h"

/**
 * @brief Gera um prinf de um timestamp
 * 
 * @param timestamp 
 */
void util_print_timestamp(uint64_t timestamp){
    if(timestamp == 0xFFFFFFFFFFFFFFFF) {
        printf("INVALID");
        return;
    }
    struct tm t;

    gmtime_r((time_t*)&timestamp, &t); // Converte para UTC
    char buffer[32];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &t);
    printf(buffer);

    //printf("(%llu) ==> %s\n", timestamp, buffer);
}

/**
 * @brief Preenche um buffer com um timestamp de forma humana
 * 
 * @param timestamp 
 * @param buf32 
 */
void util_string_timestamp(uint64_t timestamp, char *buf32){
    if(timestamp == 0xFFFFFFFFFFFFFFFF) {
        sprintf(buf32, "INVALID");
        return;
    }
    struct tm t;

    gmtime_r((time_t*)&timestamp, &t); // Converte para UTC
    strftime(buf32, 32, "%Y-%m-%d %H:%M:%S", &t);
}

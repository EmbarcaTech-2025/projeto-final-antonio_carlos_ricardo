#include <time.h>
#include <pico/cyw43_arch.h>
#include "../include/util.h"


void util_print_timestamp(uint64_t timestamp){
    struct tm t;

    gmtime_r((time_t*)&timestamp, &t); // Converte para UTC
    char buffer[32];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &t);

    printf("(%llu) ==> %s\n", timestamp, buffer);
}
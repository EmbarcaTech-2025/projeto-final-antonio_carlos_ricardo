#ifndef AHT10_H
#define AHT10_H

typedef struct{
    float humidity;
    float temp;
} Aht10Data;

void      aht10_init();
Aht10Data aht10_read();

#endif // AHT10_H
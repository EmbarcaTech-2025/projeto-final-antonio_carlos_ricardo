#ifndef GY_NEO6MV2_H
#define GY_NEO6MV2_H

typedef enum {
    GPS_GGA_NO_ERROR               =  0,
    GPS_GGA_ERROR_SIZE             = -1,
    GPS_GGA_ERROR_NO_NMEA          = -2,
    GPS_GGA_ERROR_TITLE_INVALID      = -3,
    GPS_GGA_ERROR_CHECKSUM_INVALID = -4,
    GPS_GGA_ERROR_INVALID_N_ITENS = -5,
    GPS_GGA_ERROR_NO_FIX           = -6,
    GPS_GGA_ERROR_NO_SIGNAL        = -7,
} GpsGgaError;


void gps_init();

GpsGgaError gps_read(int32_t * lat, int32_t * lon, int32_t * alt);


# endif // GY_NEO6MV2_H

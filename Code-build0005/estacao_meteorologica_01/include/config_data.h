#ifndef CONFIG_DATA_H
#define CONFIG_DATA_H

typedef enum{
    PERIOD_1SEC = 0,
    PERIOD_2SEC,
    PERIOD_5SEC,
    PERIOD_10SEC,
    PERIOD_20SEC,
    PERIOD_1MIN,
    PERIOD_2MIN,
    PERIOD_5MIN,
    PERIOD_10MIN,
    PERIOD_20MIN,
    PERIOD_1HOUR,
    PERIOD_2HOUR,
    PERIOD_6HOUR,
    PERIOD_12HOUR,
    PERIOD_1DAY
} Period;

typedef struct{
    uint32_t version;

    char station_name[16];
    char station_token[16];
    char station_group[16];
    char station_latitude[16];
    char station_longitude[16];
    char station_elevation[16];
    char wifi_ssid[32];
    char wifi_psw[16];
    char server_url[128];
    int16_t server_port;

    Period period_acquisition;
    Period period_reconnect;
    Period period_send;
    Period period_show;
    Period period_sync_clk;

    bool rtc_with;

    bool sensor_i2c_pressure;
    bool sensor_i2c_humidity;
    bool sensor_i2c_luminosity;
    bool sensor_i2c_wind_direction;

    bool sensor_digital_wind_speed;
    bool sensor_digital_rain_intensity;

    uint8_t sensor_analog_ar_co;
    uint8_t sensor_analog_ar_co2;
    uint8_t sensor_analog_ar_o3;
    uint8_t sensor_analog_soil_temperature;
    uint8_t sensor_analog_soil_humidity;
    uint8_t sensor_analog_uv_index;

    uint32_t sum;
    uint32_t xor;
} ConfigData;

#define CONFIG_DATA_VERSION         0x00000003      // quando atualizado usar versão do build

#define AD_MAX_PORT 7

#define PERIOD_ACQUISITION_MIN  PERIOD_1SEC
#define PERIOD_ACQUISITION_MAX  PERIOD_1HOUR
#define PERIOD_SEND_MIN         PERIOD_10SEC    // PERIOD_1MIN
#define PERIOD_SEND_MAX         PERIOD_1DAY

extern ConfigData config_data;

void config_data_set_default();

uint32_t config_data_get_period_in_sec(Period period);

bool config_data_storage_read();
void config_data_storage_write();

char * config_data_period_string(Period period);


#endif // CONFIG_DATA_H
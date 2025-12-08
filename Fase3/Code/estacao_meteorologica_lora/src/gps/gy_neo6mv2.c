#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "gy_neo6mv2.h"
#include "../../include/code_config.h"


#define GPS_NMEA_GPGGA_MINIMUM_SIZE     24  // $GPGGA,,,,,,,,,,,,,,*47<cr>

#define GPS_BUFFER_SIZE 128
static char gps_buffer_int[GPS_BUFFER_SIZE];
static char gps_buffer_out[GPS_BUFFER_SIZE];
static int  gps_buffer_int_idx = 0;
static int  gps_received_lines = 0;

static const uint8_t nmea_disable[] = {0x01, 0x02, 0x03, 0x04, 0x05};

static void gps_ubx_sum(uint8_t * data, uint8_t size){
    uint8_t ck_a = 0;
    uint8_t ck_b = 0;

    for(int i=2;i<(size-2);i++){
        ck_a += data[i];
        ck_b += ck_a;
    }
    data[size - 2] = ck_a;
    data[size - 1] = ck_b;
}

static void gps_disable(uint8_t nmea_f0_id){
    uint8_t disable[] = {0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

    disable[7] = nmea_f0_id;
    gps_ubx_sum(disable, sizeof(disable));
    uart_write_blocking(GPS_UART_ID, "\r\n\r\n", 4);            // Remover lixos
    uart_write_blocking(GPS_UART_ID, disable, sizeof(disable));
}


// ISR da UART
void gps_isr_uart_rx() {
//void __not_in_flash_func(gps_isr_uart_rx)() {    
    while (uart_is_readable(GPS_UART_ID)) {
        uint8_t ch = uart_getc(GPS_UART_ID);

        if(gps_buffer_int_idx >= (GPS_BUFFER_SIZE - 1)){   // estourou o buffer, recomeçar
            gps_buffer_int_idx = 0;
        }else{
            gps_buffer_int[gps_buffer_int_idx] = ch;
            if(ch =='\n'){
                gps_buffer_int[gps_buffer_int_idx-1] = 0;   // remove o <CR> 
                strcpy(gps_buffer_out, gps_buffer_int);
                gps_received_lines++;
                gps_buffer_int_idx = 0;
            }else{
                gps_buffer_int_idx++;
            }
        }
    }
}

void gps_init(){
    // Inicializa UART
    uart_init(GPS_UART_ID, GPS_UART_BAUD_RATE);
    gpio_set_function(GPS_UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(GPS_UART_RX_PIN, GPIO_FUNC_UART);
    sleep_ms(10);

    for(int i = 0;i<sizeof(nmea_disable);i++){
        gps_disable(nmea_disable[i]);
        sleep_ms(10);
    }
    
    // limpa buffer;
    while (uart_is_readable(GPS_UART_ID)) uart_getc(GPS_UART_ID);
    gps_buffer_out[0] = 0;

    // Habilita interrupção de RX
    //gps_buffer_int_idx = 0;
    //gps_received_lines = 0;
    irq_set_exclusive_handler(UART0_IRQ, gps_isr_uart_rx);
    irq_set_enabled(UART0_IRQ, true);
    uart_set_irq_enables(GPS_UART_ID, true, false);  // RX on, TX off
}



static bool gps_check_nmea_checksum(uint8_t * buffer){
    uint8_t sum = 0;
    uint8_t end = strlen(buffer) - 3;
    for(int i=1;i<end;i++){ // xor de todos os itens entre $ e *, excluido o $ e o *
        sum ^= buffer[i];
    }
    char s_sum[3];
    sprintf(s_sum, "%02X", sum);
    return strcasecmp(s_sum, &(buffer[end + 1])) == 0;
}

GpsGgaError gps_read(int32_t * lat, int32_t * lon, int32_t * alt){
    char *token_NMEA;
    char *token_time;
    char *token_lat;
    char *token_lat_ind;
    char *token_long;
    char *token_long_ind;
    char *token_fix;
    char *token_n_sate;
    char *token_hdop;
    char *token_alti;

    float   f_lat, f_lon, f_alt;
    int     tmp;
    int     error = 0;

    *lat = 0xFFFFFFFF;
    *lon = 0xFFFFFFFF;
    *alt = 0xFFFFFFFF;

    irq_set_enabled(UART0_IRQ, false);

#if (DEB_PR_CORE1_GPS == true)   
    printf("Line=%d, %s\n", gps_received_lines, gps_buffer_out);
#endif    

    
    if(!error){
        // valida tamanho mínimo de um gga
        if(strlen(gps_buffer_out) < GPS_NMEA_GPGGA_MINIMUM_SIZE) error = GPS_GGA_ERROR_SIZE;
    }

    if(!error){
        // Começa com $
        if(gps_buffer_out[0] != '$') error = GPS_GGA_ERROR_NO_NMEA;
    }

    if(!error){
        // Valida NMEA checksum
        if(!gps_check_nmea_checksum(gps_buffer_out)) error = GPS_GGA_ERROR_CHECKSUM_INVALID;
    }

    if(!error){       
        printf("GPS NMEA: %s\n", gps_buffer_out);
 
        // separa itens
        token_NMEA = strtok(gps_buffer_out, ",");
        token_time     = strtok(NULL,   ",");
        token_lat      = strtok(NULL,   ",");
        token_lat_ind  = strtok(NULL,   ",");
        token_long     = strtok(NULL,   ",");
        token_long_ind = strtok(NULL,   ",");
        token_fix      = strtok(NULL,   ",");
        token_n_sate   = strtok(NULL,   ",");    
        token_hdop     = strtok(NULL,   ",");
        token_alti     = strtok(NULL,   ",");
        if(token_alti == NULL) error = GPS_GGA_ERROR_INVALID_N_ITENS;
    }

    if(!error){        
        // valida se é GGA
        if (!strstr(token_NMEA, "$GPGGA")) error = GPS_GGA_ERROR_TITLE_INVALID;
    }

    if(!error){        
        // valida FIX
        if(*token_fix != '1') error = GPS_GGA_ERROR_NO_FIX;
    }

    if(!error){
        // read latitude
        tmp = ((token_lat[0] - '0') * 10) + (token_lat[1] - '0');
        if(sscanf(&(token_lat[2]),  "%f", &f_lat) != 1) error |= 0x01;
        else{
            f_lat = tmp + f_lat / 60.0;
        }
        switch(*token_lat_ind){
            case 'S':
            case 's':
                if(!(error & 0x01)) f_lat = -f_lat;
                break;
            case 'N':
            case 'n':
                break;
            default:
                error |= 0x02;
        }
    }

    if(!error){        
        // read longitude
        tmp = ((token_long[0] - '0') * 100) + ((token_long[1] - '0') * 10) + (token_long[2] - '0');
        if(sscanf(&(token_long[3]),  "%f", &f_lon) != 1) error |= 0x04;
        else{
            f_lon = tmp + f_lon / 60.0;
        }
        switch(*token_long_ind){
            case 'W':
            case 'w':
                if(!(error & 0x04)) f_lon = -f_lon;
                break;
            case 'E':
            case 'e':
                break;
            default:
                error |= 0x08;
        }
    }

    if(!error){        
        // read altitude
        if(sscanf(token_alti, "%f", &f_alt) != 1) error |= 0x10;
    }



    irq_set_enabled(UART0_IRQ, true);

    if(error) return error;

    *lat = f_lat * 0x00800000;
    *lon = f_lon * 0x00800000;
    *alt = f_alt * 0x00010000;

    

    
    printf("\n");
    printf("token_NMEA    :%s\n", token_NMEA    );
    printf("token_time    :%s\n", token_time    );
    printf("token_lat     :%s\n", token_lat     );
    printf("token_lat_ind :%s\n", token_lat_ind );
    printf("token_long    :%s\n", token_long    );
    printf("token_long_ind:%s\n", token_long_ind);
    printf("token_fix     :%s\n", token_fix     );
    printf("token_n_sate  :%s\n", token_n_sate  );
    printf("token_hdop    :%s\n", token_hdop    );
    printf("token_alti    :%s\n", token_alti    );

    printf("f_lat : %f\n", f_lat    );
    printf("f_lon : %f\n", f_lon    );
    printf("f_alt : %f\n", f_alt    );

    printf("lat : %ld\n", *lat    );
    printf("lon : %ld\n", *lon    );
    printf("alt : %ld\n", *alt    );


    return GPS_GGA_NO_ERROR;    // Sinal é válido
    
    
}


#include <stdio.h>
#include "pico/stdlib.h"
#include <pico/cyw43_arch.h>
#include <lwip/tcp.h>

#include "../include/config.h"
#include "../include/config_data.h"
#include "../include/estation_data.h"
#include "../include/radio.h"
#include "../include/wifi.h"

bool wifi_connected   = false;

#define POST_PATH   "/dados"


void radio_init(){
    wifi_connected = false;

    gpio_put(LED_R_GPIO, true);
    gpio_put(LED_G_GPIO, false);
    gpio_put(LED_B_GPIO, false);
}

void radio_update(){
    if(wifi_connected){
        //cyw43_arch_poll();
        if(cyw43_tcpip_link_status(&cyw43_state, CYW43_ITF_STA) == CYW43_LINK_UP){
                    // minhas rotinas com wifi conectado
                    //broker_connected = mqtt_base_update();
        }else{
            wifi_connected = false;
            //broker_connected = false;
            //        mqtt_base_disconnect();
            sleep_ms(WIFI_AFTER_CONNECT_FAIL_DELAY_MS);
        }
    }else{
        printf("WiFi desconnected\n");
        wifi_connected = wifi_connect_or_reconnected();
        if (!wifi_connected) {
            sleep_ms(WIFI_AFTER_CONNECT_FAIL_DELAY_MS);  // Pequena espera antes de tentar de novo
        }
    }

    gpio_put(LED_R_GPIO, !wifi_connected);
    gpio_put(LED_G_GPIO, wifi_connected);
    gpio_put(LED_B_GPIO, false);
}

bool radio_get_time(uint64_t * t){
    return false;
}



static bool retornou = false;
static char radio_msg[256+128];

// Dica: deixe o servidor colocar o timestamp verdadeiro.
// Se quiser usar NTP, habilite SNTP no lwIP. Aqui usamos time() mesmo.
static void iso8601_utc_now(char *buf, size_t bufsz) {
    time_t now = time(NULL);
    struct tm tm_utc;
    gmtime_r(&now, &tm_utc);
    strftime(buf, bufsz, "%Y-%m-%dT%H:%M:%SZ", &tm_utc);
}


struct post_state {
    struct tcp_pcb *pcb;
    char buffer[512];
    int sent_len;
    int total_len;
};

static err_t tcp_sent_cb(void *arg, struct tcp_pcb *tpcb, u16_t len) {
    struct post_state *st = arg;
    st->sent_len += len;
    if (st->sent_len >= st->total_len) {
        printf("Envio concluído!\n");
        tcp_close(tpcb);
    }
    return ERR_OK;
}

static err_t tcp_recv_cb(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err) {
    if (!p) { tcp_close(tpcb); return ERR_OK; }
    printf("Resposta recebida (%d bytes)\n", p->tot_len);
    pbuf_free(p);
    return ERR_OK;
}



static err_t tcp_connected_cb(void *arg, struct tcp_pcb *tpcb, err_t err) {
    if (err != ERR_OK) return err;
    struct post_state *st = arg;

    float temp  = 20.0   + ((float)rand()) / RAND_MAX * 15.0;
    float press = 1000.1 + ((float)rand()) / RAND_MAX * 50.0;
    int   humi  = 50.0   + (((uint64_t)rand()) * 40) / RAND_MAX ;
    
    char s_t[32];
    iso8601_utc_now(s_t, 31) ;

    // Monta o POST
    //sprintf(json_tmp, "{\"id\":\"pico003\",\"time\":\"2025-08-14T12:00:00Z\",\"sensors\":{\"temp\":%.1f,\"press\":%.1f,\"humi\":%d}}",

    /*
    sprintf(radio_msg, "{\"id\":\"pico003\",\"time\":\"%s\",\"sensors\":{\"temp\":%.1f,\"press\":%.1f,\"humi\":%d}}",
        s_t,
        temp,
        press,
        humi
    );*/
    //const char *json = "{\"id\":\"pico001\",\"time\":\"2025-08-14T12:00:00Z\",\"sensors\":{\"temp\":21.5,\"press\":1012.3,\"humi\":55}}";
    //int json_len = strlen(json);


    int json_len = strlen(radio_msg);
    st->total_len = snprintf(st->buffer, sizeof(st->buffer),
        "POST %s HTTP/1.1\r\n"
        "Host: %s:%d\r\n"
        "Content-Type: application/json\r\n"
        "Content-Length: %d\r\n"
        "Connection: close\r\n"
        "\r\n"
        "%s",
//        POST_PATH, SERVER_IP, SERVER_PORT, json_len, json);
        POST_PATH, config_data.server_url, config_data.server_port, json_len, radio_msg);

    st->sent_len = 0;

    tcp_recv(tpcb, tcp_recv_cb);
    tcp_sent(tpcb, tcp_sent_cb);

    err_t wr_err = tcp_write(tpcb, st->buffer, st->total_len, TCP_WRITE_FLAG_COPY);
    if (wr_err != ERR_OK) {
        printf("tcp_write falhou: %d\n", wr_err);
        return wr_err;
    }
    tcp_output(tpcb);
    return ERR_OK;
}







bool radio_send_data(EstationData * estation_data){
    if(!wifi_connected) return false;


    char s_t[32];
    //iso8601_utc_now(s_t, 31) ;


    struct tm tm_utc;
    gmtime_r(&(estation_data->t_aquire), &tm_utc);
    strftime(s_t, 31, "%Y-%m-%dT%H:%M:%SZ", &tm_utc);


    // Monta o POST
    sprintf(radio_msg, "{\"id\":\"%s\",\"time\":\"%s\",\"sensors\":{\"temp\":%.1f,\"press\":%.1f,\"humi\":%d}}",
        config_data.station_name,
        s_t,
        estation_data->temp_p   * 0.01,
        estation_data->press    * 0.01,
        estation_data->humidity
    );




    retornou = false;

    ip4_addr_t dest_ip;
    //ip4addr_aton(SERVER_IP, &dest_ip);
    ip4addr_aton(config_data.server_url, &dest_ip);

    struct tcp_pcb *pcb = tcp_new_ip_type(IPADDR_TYPE_V4);
    static struct post_state st;
    st.pcb = pcb;

    tcp_arg(pcb, &st);
    //tcp_connect(pcb, &dest_ip, SERVER_PORT, tcp_connected_cb);
    tcp_connect(pcb, &dest_ip, config_data.server_port, tcp_connected_cb);


    retornou = true;

    uint64_t timeout = time_us_64() + 10 * 1000000;
    while((time_us_64()<timeout)&&(!retornou)){
        // refresh Wifi
        cyw43_arch_poll();

        sleep_ms(10);
    }
    return retornou;
}



/*
    if (cyw43_arch_init()) {
        printf("Falha ao iniciar Wi-Fi\n");
        return -1;
    }
    cyw43_arch_enable_sta_mode();

    printf("Conectando Wi-Fi...\n");
    if (cyw43_arch_wifi_connect_timeout_ms(
        WIFI_SSID, WIFI_PASS, CYW43_AUTH_WPA2_AES_PSK, 30000)) {
        printf("Falha na conexão Wi-Fi\n");
        return -1;
    }
    printf("Wi-Fi conectado!\n");


    cyw43_arch_poll();  // processa eventos de rede


*/
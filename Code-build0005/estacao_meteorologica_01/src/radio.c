#include <stdio.h>
#include "pico/stdlib.h"
#include <pico/cyw43_arch.h>
#include <lwip/tcp.h>

#include "../include/config.h"
#include "../include/config_data.h"
#include "../include/estation_data.h"
#include "../include/radio.h"
#include "../include/radio_common.h"
#include "../include/radio_get_time.h"
#include "../include/radio_send_data.h"
#include "../include/wifi.h"

bool wifi_connected   = false;

#define POST_PATH_SEND_DATA   "/dados"
#define POST_PATH_GET_TIME    "/hora"





struct post_state {
    struct tcp_pcb *pcb;
    char buffer[512];
    int sent_len;
    int total_len;
};


void radio_init(){
    wifi_connected = false;
    radio_post_data.radio_post_state = RADIO_POST_ST_IDLE;
}

bool radio_update(){
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

    return wifi_connected;
}


bool radio_connected(){
    return wifi_connected;
}

/*

bool radio_get_time(uint64_t * t){
    return false;
}*/



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
//        POST_PATH_SEND_DATA, SERVER_IP, SERVER_PORT, json_len, json);
        POST_PATH_SEND_DATA, config_data.server_url, config_data.server_port, json_len, radio_msg);

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







bool radio_send_est_data(char * json_msg){
    if(!wifi_connected) return false;
    //int16_t size = estation_data_json(estation_data, estation_data_buf_tmp);
    //int16_t size = estation_data_json_full_fake(estation_data, estation_data_buf_tmp);
    return radio_send_data(json_msg);
}

bool radio_get_time_r(uint64_t * t){
    return radio_get_time(t);
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


/*
Estou fazendo um programa em C para o microcontrolador RP2040.
Este programa roda na placa Raspberry pico_w.
Vou utilizar o módulo WiFI. Para isso inclui a lib: pico_cyw43_arch_lwip_threadsafe_background
Estando programa pode fazer 2 tipos de requisição post para o servidor:
-Enviar um Dado 
-Requerer a hora atual.
Só poderá ser feita tentativas de requisição quando o WiFi da placa estiver conectado com o Roteador WiFi.
No main loop do programa é monitorado o WiFi constantemente e caso se verifique que caiu a conexão entre a placa e o retoador WiFi tenta-se fazer sucessivamente a reconexão.

Como a conexão entre Placa e Roteador e entre Placa e Servidor pode cair por qualquer razão, o algoritimo proposto será para cada requisição a ser efetuada se abrir uma nova conexão e ao final desconectar.

terei comos dados que serão inicializados apenas na inicialização do programa o seguinte:

typedef enum{
    RADIO_POST_ST_IDLE = 0,
    RADIO_POST_ST_DATA_CONNECTING,
    RADIO_POST_ST_DATA_SENDING,
    RADIO_POST_ST_DATA_SENT_OK,
    RADIO_POST_ST_DATA_SEND_ERROR,
    RADIO_POST_ST_TIME_REQUEST_CONNECTING,
    RADIO_POST_ST_TIME_REQUEST_SENDING,
    RADIO_POST_ST_TIME_REQUEST_RECEIVED,
    RADIO_POST_ST_TIME_REQUEST_ERROR,
} RadioPostState;

static ip4_addr_t radio_dest_ip;
static struct {
    struct tcp_pcb *pcb;
    char           *buffer;
    int             sent_len;
    int             total_len;
    bool            busy;
    uint64_t        start_post;
    RadioPostState  radio_post_state;
} radio_post_data;

void rotina_de_inicializacao_unica(){
    
    
}


Ai terei 2 rotinas que só podem ser chamadas quando os post estivar em IDLE, radio_send_data e radio_get_time.
No momento vamos analisar primeiro a radio_send_data e apenas uma parte



static err_t tcp_connected_send_data(void *arg, struct tcp_pcb *tpcb, err_t err) {
    // continuarimos depois

}



---------------------------------------------------------------------------------------------------------










XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

Estando conectado meu programa pode fazer 2 tipos de requisição post com o servidor:
-Enviar um Dado 
-Requerer a hora atual.
Há variáveis que não devem alterar mesmo que a conexão caia e o programa faça a reconexão, creio que são as variáveis:



*/
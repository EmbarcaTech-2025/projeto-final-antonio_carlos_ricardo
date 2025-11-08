/**
 * @file    wifi_tcp.c
 * @author  Carlos e Ricardo
 * @brief   Executa requisições/envio de mensagens TCP
 * @version 0.1
 * @date    2025-09-05
 * 
 * @copyright Copyright (c) 2025
 */
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "lwip/pbuf.h"
#include "lwip/tcp.h"
#include "../include/wifi_tcp.h"


// precisa ser revista a linha:   "Host: " TEST_TCP_SERVER_IP ":%d\r\n"

#define POLL_TIME_500MS     10      // tempo para chamada do poll, que corresposde ao valor * 500ms, portanto 10 ==> 5 seg


/**
 * @brief Estrutura utizada para o protocolo TCP
 * 
 */
static WifiTcpData wifi_tcp_data;


/**
 * @brief Rotina de encerramento do protocolo
 * - Libera as configurações
 *
 * @param wtd 
 * @return err_t 
 */
static err_t tcp_client_close(WifiTcpData * wtd) {
    err_t err = ERR_OK;

    if(wtd->pcb != NULL){
        tcp_arg( wtd->pcb, NULL);
        tcp_poll(wtd->pcb, NULL, 0);
        tcp_sent(wtd->pcb, NULL);
        tcp_recv(wtd->pcb, NULL);
        tcp_err( wtd->pcb, NULL);

        err = tcp_close(wtd->pcb);
        if (err != ERR_OK) {
            tcp_abort(wtd->pcb);
            err = ERR_ABRT;
        }
        wtd->pcb = NULL;
    }
    return err;
}

/**
 * @brief Callback chamado quando há um erro durante a execução do protocolo TCP
 * - o protocolo é encerrado/concluído com ERRO
 * 
 * @param arg 
 * @param err 
 */
static void  tcp_callback_err( void *arg, err_t err) {
    ((WifiTcpData*)arg)->wifi_tcp_st = WIFI_TCP_ST_ERROR;
}

/**
 * @brief Callback chamado quando decorrido um tempo sem atividade na comunicação
 * - no caso indica o timeout do servidor
 * - o protocolo é encerrado/concluído com ERRO
 * 
 * @param arg 
 * @param tpcb 
 * @return err_t 
 */
static err_t tcp_callback_poll(void *arg, struct tcp_pcb *tpcb){
    ((WifiTcpData*)arg)->wifi_tcp_st = WIFI_TCP_ST_POOL;
    return tcp_client_close((WifiTcpData*)arg); // no response is an error?
}

/**
 * @brief Callback chamado quando se recebe dados
 * - o Dado é recebido, colocado no buffer
 * - o protocolo é encerrado/concluído com OK
 * 
 * @param arg 
 * @param tpcb 
 * @param p 
 * @param err 
 * @return err_t 
 */
static err_t tcp_callback_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err) {
    WifiTcpData * wtd = (WifiTcpData *)arg;

    if (!p) {
        wtd->wifi_tcp_st = WIFI_TCP_ST_RECEIVED_CLOSE;
        return tcp_client_close(wtd);
    }

    // limpar o buffer, para garantir que o último caractere seja 0x00
    memset(wifi_tcp_data.buffer, 0x00, sizeof(wifi_tcp_data.buffer));

    // this method is callback from lwIP, so cyw43_arch_lwip_begin is not required, however you
    // can use this method to cause an assertion in debug mode, if this method is called when
    // cyw43_arch_lwip_begin IS needed
    cyw43_arch_lwip_check();

    // verifica/seta o tamanho máximo de buffer e copia os dados
    int16_t size = p->len;
    if(size >= sizeof(wifi_tcp_data.buffer)) size = sizeof(wifi_tcp_data.buffer) - 1;
    memcpy(wifi_tcp_data.buffer, p->payload, size);
    wtd->buffer_len = size;

    // da baixa nos dados recebidos
    tcp_recved(tpcb, p->len);

    // libera o buffer
    pbuf_free(p);

    wtd->wifi_tcp_st = WIFI_TCP_ST_RECEIVED;
    tcp_client_close(wtd);

    return ERR_OK;
}

/**
 * @brief Callback chamado quando se está enviando os dados
 * - Indica quantos dados já foram enviados
 * - o protocolo não se encerra pois é necessário receber a resposta do servidor
 * - gera erro e encerra/concluí com ERRO caso se tenta enviar mais dados doque os que tinham na pilha, (quando ocorre?)
 * 
 * @param arg 
 * @param tpcb 
 * @param len 
 * @return err_t 
 */
static err_t tcp_callback_sent(void *arg, struct tcp_pcb *tpcb, u16_t len) {
    WifiTcpData * wtd = (WifiTcpData *)arg;
    
    wtd->sent_len += len;

    if (wtd->sent_len >= WIFI_TCP_BUFFER_SIZE) {
       wtd->wifi_tcp_st = WIFI_TCP_ST_SENDING_ERROR;
       return tcp_client_close(wtd);
    }
    wtd->wifi_tcp_st = WIFI_TCP_ST_SENDING;
    return ERR_OK;
}


/**
 * @brief Callback chamado quando uma conexão com o servidor foi estabelecida
 * - Faz a requisição de envio da mensagem para o servidor
 * 
 * @param arg 
 * @param tpcb 
 * @param err 
 * @return err_t 
 */
static err_t tcp_callback_connected(void *arg, struct tcp_pcb *tpcb, err_t err) {
    WifiTcpData * wtd = (WifiTcpData *)arg;

    wtd->wifi_tcp_st = WIFI_TCP_ST_CONNECTED;

    if (err != ERR_OK) {
        wtd->wifi_tcp_st = WIFI_TCP_ST_CONNECTED_ERROR;
        tcp_client_close(wtd);
        return err;
    }

    // Montando o dado para enviar no buffer
    int len = snprintf(wtd->buffer, sizeof(wtd->buffer),
        "POST %s HTTP/1.1\r\n"
        //"Host: " TEST_TCP_SERVER_IP ":%d\r\n"
        "Host: %s:%u\r\n"
        "Content-Type: application/json\r\n"
        "Content-Length: %d\r\n"
        "\r\n"
        "%s",
        wtd->page,  wtd->url,  wtd->port, strlen(wtd->json), wtd->json);

    wtd->buffer_len = len;
    wtd->sent_len   = 0;

    // Envia o dado
    err_t err_send = tcp_write(tpcb, wtd->buffer, wtd->buffer_len, TCP_WRITE_FLAG_COPY);
    if (err_send != ERR_OK) {
        wtd->wifi_tcp_st = WIFI_TCP_ST_CONNECTED_ERROR;
        return tcp_client_close(wtd);
    }
    tcp_output(tpcb);

    return ERR_OK;
}

/**
 * @brief Dispara a tentativa de conexão
 * - configura a estrutura de dados para o protocolo TCP
 * - dispara para iniciar a tentativa de conexão
 * 
 * @param wtd 
 * @return true, Tentativa de conexão disparada
 * @return false, falha na tentativa de conexão
 */
static bool tcp_start_connection(WifiTcpData * wtd) {
    wtd->pcb = tcp_new_ip_type(IP_GET_TYPE(&state->remote_addr));
    if (!wtd->pcb) {
        return false;
    }

    tcp_arg( wtd->pcb, wtd);
    tcp_poll(wtd->pcb, tcp_callback_poll, POLL_TIME_500MS);
    tcp_sent(wtd->pcb, tcp_callback_sent);
    tcp_recv(wtd->pcb, tcp_callback_recv);
    tcp_err( wtd->pcb, tcp_callback_err);

    wtd->buffer_len = 0;

    wtd->wifi_tcp_st = WIFI_TCP_ST_CONNECTING;

    // cyw43_arch_lwip_begin/end should be used around calls into lwIP to ensure correct locking.
    // You can omit them if you are in a callback from lwIP. Note that when using pico_cyw_arch_poll
    // these calls are a no-op and can be omitted, but it is a good practice to use them in
    // case you switch the cyw43_arch type later.
    cyw43_arch_lwip_begin();
    err_t err = tcp_connect(wtd->pcb, &wtd->remote_addr, wtd->port, tcp_callback_connected);
    cyw43_arch_lwip_end();

    return err == ERR_OK;
}



/**
 * @brief Indica se o protocolo TCP foi encerrado
 * 
 * @param wifi_tcp_st 
 * @return true, protocolo TCP encerrado
 * @return false, está no meio do protocolo 
 */
bool wifi_tcp_end(WifiTcpState wifi_tcp_st){
    if(wifi_tcp_st == WIFI_TCP_ST_CONNECTED_ERROR) return true;
    if(wifi_tcp_st == WIFI_TCP_ST_SENDING_ERROR)   return true;
    if(wifi_tcp_st == WIFI_TCP_ST_RECEIVED)        return true;
    if(wifi_tcp_st == WIFI_TCP_ST_RECEIVED_CLOSE)  return true;
    if(wifi_tcp_st == WIFI_TCP_ST_ERROR)           return true;
    if(wifi_tcp_st == WIFI_TCP_ST_POOL)            return true;
    return false;
}

/**
 * @brief Envia uma requisição TCP e aguarda a resposta
 * 
 * @param ip 
 * @param url 
 * @param porta 
 * @param page 
 * @param json 
 * 
 * @return WifiTcpData*, ponteiro da estrutura que contem a resposta, null se houve erro grave 
 */
WifiTcpData * wifi_tcp_send(ip_addr_t ip, char * url, uint16_t port, char * page, char * json){
    wifi_tcp_data.wifi_tcp_st = WIFI_TCP_ST_PRE_START;
    wifi_tcp_data.remote_addr = ip;
    wifi_tcp_data.url         = url;
    wifi_tcp_data.port        = port;
    wifi_tcp_data.page        = page;
    wifi_tcp_data.json        = json;
    wifi_tcp_data.pcb         = NULL;
    memset(wifi_tcp_data.buffer, 0x00, sizeof(wifi_tcp_data.buffer));
    wifi_tcp_data.buffer_len  = 0;
    wifi_tcp_data.sent_len    = 0;


    // Dispara a tentativa de conexão
    if(!tcp_start_connection(&wifi_tcp_data)) {
        tcp_client_close(&wifi_tcp_data);
        return NULL;
    }

    // Aguarda o fim do protocolo
    while(!wifi_tcp_end(wifi_tcp_data.wifi_tcp_st)) {
        //sleep_ms(1000);
        cyw43_arch_poll();
    }
    return &wifi_tcp_data;
}

/**
 * @brief Verifica se a mensagem recebida é a 200 que indica que o servidor recebeu corretamente um dado
 * 
 * @param wifi_tcp_data 
 * @return true, Servidor recebeu corretamente o dado
 * @return false, Houve erro no processo de envio ou recebimento do dado pelo servidor
 */
bool wifi_tcp_answer_send(    WifiTcpData * wifi_tcp_data){
    if(wifi_tcp_data == NULL) return false;
    if(wifi_tcp_data->wifi_tcp_st != WIFI_TCP_ST_RECEIVED) return false;

    // o buffer é terminado com 0, pela rotina tcp_callback_recv

    if (strstr(wifi_tcp_data->buffer, "HTTP/1.1 200") || strstr(wifi_tcp_data->buffer, "HTTP/1.0 200")) {
        return true;
    } else {
        return false;
    }
}

/**
 * @brief Verifica se a mensagem recebida do servidor é uma data/hora válida
 * 
 * @param wifi_tcp_data 
 * @return true, Servidor enviou uma data/hora válida que foi gravada no ponteiro t
 * @return false, Houve erro no processo de envio ou recebimento do dado pelo servidor
 */
bool wifi_tcp_answer_get_time(WifiTcpData * wifi_tcp_data, uint64_t *t_ret){
    if(!wifi_tcp_answer_send(wifi_tcp_data)) return false;

    char *body = strstr(wifi_tcp_data->buffer, "\r\n\r\n");
    if (!body) {
        return false;   //printf("Resposta malformada (sem cabeçalhos).\n");
    }

    body += 4;
    // Procurar a chave "time":
    char *pos_time = strstr(body, "\"time\":");
    if (!pos_time) {
        return false; //printf("Campo 'time' não encontrado.\n");
    }

    // Extrair o valor com sscanf, formato: "time":"2025-08-29T21:28:54.422061774Z"
    int ano, mes, dia, hora, minuto, segundo;
    int n = sscanf(pos_time, "\"time\":\"%d-%d-%dT%d:%d:%d", &ano, &mes, &dia, &hora, &minuto, &segundo);
    struct tm t = {0};
    t.tm_year = ano - 1900; // anos desde 1900
    t.tm_mon  = mes - 1;    // meses de 0 a 11
    t.tm_mday = dia;
    t.tm_hour = hora;
    t.tm_min  = minuto;
    t.tm_sec  = segundo;
    *t_ret    = (uint64_t)mktime(&t);
    return true;
}
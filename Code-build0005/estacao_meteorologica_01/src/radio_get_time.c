#include <stdio.h>
#include "pico/stdlib.h"
#include <pico/cyw43_arch.h>
#include <lwip/tcp.h>
#include "../include/config.h"
#include "../include/config_data.h"
#include "../include/radio_common.h"
#include "../include/radio_get_time.h"

static uint64_t time_ret;

static void tcp_get_time_err_callback(void *arg, err_t err){
    if(DEB_PR_CORE0_WIFI_GET_TIME) printf("tcp_get_time_err_callback\n");
    // Isso pode ser chamado sem chance de resposta posterior, então apenas atualize o estado.
    radio_post_data.radio_post_state = RADIO_POST_ST_TIME_REQUEST_ERROR;
    // não use tcp_abort(), nem tcp_close(), pois pcb já está inutilizável
    radio_post_data.pcb = NULL; // pcb já foi liberado pelo lwIP internamente nesse ponto
}
static err_t tcp_get_time_recv_callback(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err) {
    if(DEB_PR_CORE0_WIFI_GET_TIME) printf("tcp_get_time_recv_callback\n");
    if (!p) {   // Conexão foi encerrada pelo servidor
        radio_post_data.radio_post_state = RADIO_POST_ST_TIME_REQUEST_ERROR;
        //tcp_close(tpcb);
        tcp_abort(tpcb);
        radio_post_data.pcb = NULL;
        return ERR_OK;
    }

    // Aqui você poderia processar a resposta, se necessário
    // Verifica se a resposta HTTP contém "200 OK"
    char resp[32] = {0};
    int size = p->tot_len > 31? 31 : (p->tot_len - 1);
    memcpy(resp, p->payload, size);
    if (strstr(resp, "HTTP/1.1 200") || strstr(resp, "HTTP/1.0 200")) {
        radio_post_data.radio_post_state = RADIO_POST_ST_DATA_SENT_OK;
        
        char *body = strstr(p->payload, "\r\n\r\n");
        if (!body) {
            radio_post_data.radio_post_state = RADIO_POST_ST_TIME_REQUEST_ERROR;
            //printf("Resposta malformada (sem cabeçalhos).\n");
        }else{
            body += 4;
            // Procurar a chave "time":
            char *pos_time = strstr(body, "\"time\":");
            if (!pos_time) {
                //printf("Campo 'time' não encontrado.\n");
                radio_post_data.radio_post_state = RADIO_POST_ST_TIME_REQUEST_ERROR;
            }else{
                // Extrair o valor com sscanf
                //2025-08-29T21:28:54.422061774Z
                // "time":"2025-08-29T21:28:54.422061774Z"
                int ano, mes, dia, hora, minuto, segundo;
                int n = sscanf(pos_time, "\"time\":\"%d-%d-%dT%d:%d:%d", &ano, &mes, &dia, &hora, &minuto, &segundo);
                struct tm t = {0};
                t.tm_year = ano - 1900; // anos desde 1900
                t.tm_mon  = mes - 1;    // meses de 0 a 11
                t.tm_mday = dia;
                t.tm_hour = hora;
                t.tm_min  = minuto;
                t.tm_sec  = segundo;
                time_ret = (uint64_t)mktime(&t);
                radio_post_data.radio_post_state = RADIO_POST_ST_TIME_REQUEST_RECEIVED_OK;
                /*
                if (sscanf(pos_time, "\"time\":\"%" SCNu64 "\"", &time_ret) == 1) {
                    radio_post_data.radio_post_state = RADIO_POST_ST_TIME_REQUEST_RECEIVED_OK;
                } else {
                    //printf("Falha ao extrair valor de 'time'.\n");
                    radio_post_data.radio_post_state = RADIO_POST_ST_TIME_REQUEST_ERROR;
                }*/
            }
        }
    
    } else {
        radio_post_data.radio_post_state = RADIO_POST_ST_TIME_REQUEST_ERROR;
    }

    tcp_recved(tpcb, p->tot_len);       // Sempre dar ack do pbuf recebido
    pbuf_free(p);                       // Libera memória

    tcp_arg(  tpcb, NULL);
    tcp_sent( tpcb, NULL);
    tcp_recv( tpcb, NULL);
    tcp_err(  tpcb, NULL);
    //tcp_close(tpcb);
    tcp_abort(tpcb);
    radio_post_data.pcb = NULL;

    return ERR_OK;
}
static err_t tcp_get_time_sent_callback(void *arg, struct tcp_pcb *tpcb, u16_t len) {
    if(DEB_PR_CORE0_WIFI_GET_TIME) printf("tcp_get_time_sent_callback\n");
    // vou aguardar a resposta do servidor
    /*
    struct post_state *st = arg;
    st->sent_len += len;
    if (st->sent_len >= st->total_len) {
        printf("Envio concluído!\n");
        tcp_close(tpcb);
    }*/
    return ERR_OK;
}


static err_t tcp_connected_get_time(void *arg, struct tcp_pcb *tpcb, err_t err) {
    if(DEB_PR_CORE0_WIFI_GET_TIME) printf("tcp_connected_send_data\n");
    if (err != ERR_OK) {
        radio_post_data.radio_post_state = RADIO_POST_ST_TIME_REQUEST_ERROR;

        // Libera a conexão se ela foi parcialmente aberta
        if (tpcb) {
            tcp_abort(tpcb);
            radio_post_data.pcb = NULL;
        }
        return err;
    }

    // A conexão foi estabelecida, agora podemos começar a enviar os dados
    radio_post_data.radio_post_state = RADIO_POST_ST_TIME_REQUEST_SENDING;
    radio_post_data.sent_len         = 0;

    // Define Callbacks
    tcp_err( tpcb, tcp_get_time_err_callback);
    tcp_recv(tpcb, tcp_get_time_recv_callback);
    tcp_sent(tpcb, tcp_get_time_sent_callback); // você implementa esse para marcar que tudo foi enviado
    
    // Escreve a mensagem
    err_t write_err = tcp_write(tpcb, radio_post_data.buffer, radio_post_data.total_len, TCP_WRITE_FLAG_COPY);
    if (write_err != ERR_OK) {
        radio_post_data.radio_post_state = RADIO_POST_ST_TIME_REQUEST_ERROR;
        tcp_abort(tpcb);
        radio_post_data.pcb = NULL;
        return write_err;
    }
    tcp_output(tpcb);
    
    return ERR_OK;
}

bool radio_get_time(uint64_t * t){
    if(DEB_PR_CORE0_WIFI_GET_TIME) printf("radio_get_time\n");
    if (radio_post_data.radio_post_state != RADIO_POST_ST_IDLE) return false;

    // GERAR A chamada

    char jsont_to_send[32] = {0};
    sprintf(jsont_to_send, "{\"id\":\"%s\"}", config_data.station_name);
    
    int json_len = strlen(jsont_to_send);

    
    radio_post_data.total_len = snprintf(radio_post_data.buffer, sizeof(radio_post_data.buffer),
        "POST %s HTTP/1.1\r\n"
        "Host: %s:%d\r\n"
        "Content-Type: application/json\r\n"
        "Content-Length: %d\r\n"
        "Connection: close\r\n"
        "\r\n"
        "%s",
        POST_PATH_GET_TIME, config_data.server_url, config_data.server_port, json_len, jsont_to_send);



    // Cria nova conexão
    if (radio_post_data.pcb) {
        tcp_abort(radio_post_data.pcb); // Garante conexão limpa
    }
    radio_post_data.pcb = tcp_new_ip_type(IPADDR_TYPE_V4);
    if (!radio_post_data.pcb) {
        return false;
    }

    // define o argumento que será passado
    tcp_arg(radio_post_data.pcb, &radio_post_data);
    
    // Inicia a conexão de fato
    ip4_addr_t radio_dest_ip;
    ip4addr_aton(config_data.server_url, &radio_dest_ip);

    radio_post_data.sent_len         = 0;
    radio_post_data.radio_post_state = RADIO_POST_ST_TIME_REQUEST_CONNECTING;
    radio_post_data.start_post       = time_us_64();
    err_t err = tcp_connect(radio_post_data.pcb, &radio_dest_ip, config_data.server_port, tcp_connected_get_time);
    if (err != ERR_OK) {
        tcp_abort(radio_post_data.pcb);
        radio_post_data.pcb = NULL;
        radio_post_data.radio_post_state = RADIO_POST_ST_IDLE;
        return false;
    }

    // verifica envio ou TimeOut
    uint64_t timeout = time_us_64() + POST_TIMEOUT_US;
    while((time_us_64() < timeout) && 
          (radio_post_data.radio_post_state != RADIO_POST_ST_IDLE) && 
          (radio_post_data.radio_post_state != RADIO_POST_ST_TIME_REQUEST_RECEIVED_OK) && 
          (radio_post_data.radio_post_state != RADIO_POST_ST_TIME_REQUEST_ERROR)){
        sleep_us(10000);
        cyw43_arch_poll();
    }

    if (radio_post_data.pcb) {
        tcp_abort(radio_post_data.pcb);
        radio_post_data.pcb = NULL;
    }

    bool ret = radio_post_data.radio_post_state == RADIO_POST_ST_TIME_REQUEST_RECEIVED_OK;
    if(ret){
        *t = time_ret;
    }
    radio_post_data.radio_post_state = RADIO_POST_ST_IDLE;
    return ret;
}




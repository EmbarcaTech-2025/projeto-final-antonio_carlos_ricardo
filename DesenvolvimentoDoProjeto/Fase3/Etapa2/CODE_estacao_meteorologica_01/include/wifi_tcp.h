/**
 * @file    wifi_tcp.h
 * @author  Carlos e Ricardo
 * @brief   Executa requisições/envio de mensagens TCP
 * @version 0.1
 * @date    2025-09-05
 * 
 * @copyright Copyright (c) 2025
 */
#ifndef WIFI_TCP_H
#define WIFI_TCP_H


# define WIFI_TCP_BUFFER_SIZE   1024

/**
 * @brief Estado que esta o protocolo TCP
 */
typedef enum {
    WIFI_TCP_ST_PRE_START = 0,
    WIFI_TCP_ST_CONNECTING,
    WIFI_TCP_ST_CONNECTED,
    WIFI_TCP_ST_CONNECTED_ERROR,
    WIFI_TCP_ST_SENDING,
    WIFI_TCP_ST_SENDING_ERROR,
    WIFI_TCP_ST_RECEIVED,
    WIFI_TCP_ST_RECEIVED_CLOSE,
    WIFI_TCP_ST_ERROR,
    WIFI_TCP_ST_POOL,
} WifiTcpState;    

/**
 * @brief Estrutura necessária para o protocolo TCP
 */
typedef struct {
    WifiTcpState    wifi_tcp_st;
    ip_addr_t       remote_addr;
    char           *url;
    uint16_t        port;
    char           *page;
    char           *json;                           // Requisição a ser transmitida
    struct tcp_pcb *pcb;
    uint8_t         buffer[WIFI_TCP_BUFFER_SIZE];   // Buffer para uso na transmissão e recepção dos dados
    int             buffer_len;                     // número de dados no buffer
    int             sent_len;                       // Dados que já foram enviados
} WifiTcpData;

/**
 * @brief Indica se o protocolo TCP foi encerrado
 * 
 * @param wifi_tcp_st 
 * @return true, protocolo TCP encerrado
 * @return false, está no meio do protocolo 
 */
bool wifi_tcp_end(WifiTcpState wifi_tcp_st);

/**
 * @brief Envia uma requisição TCP e aguarda a resposta
 * 
 * @param ip //ip4addr_aton(TEST_TCP_SERVER_IP, &ip);
 * @param url 
 * @param porta 
 * @param page 
 * @param json 
 * 
 * @return WifiTcpData*, ponteiro da estrutura que contem a resposta, null se houve erro grave 
 */
WifiTcpData * wifi_tcp_send(ip_addr_t ip, char * url, uint16_t porta, char * page, char * json);

/**
 * @brief Verifica se a mensagem recebida é a 200 que indica que o servidor recebeu corretamente um dado
 * 
 * @param wifi_tcp_data 
 * @return true, Servidor recebeu corretamente o dado
 * @return false, Houve erro no processo de envio ou recebimento do dado pelo servidor
 */
bool          wifi_tcp_answer_send(    WifiTcpData * wifi_tcp_data);

/**
 * @brief Verifica se a mensagem recebida do servidor é uma data/hora válida
 * 
 * @param wifi_tcp_data 
 * @return true, Servidor enviou uma data/hora válida que foi gravada no ponteiro t
 * @return false, Houve erro no processo de envio ou recebimento do dado pelo servidor
 */
bool          wifi_tcp_answer_get_time(WifiTcpData * wifi_tcp_data, uint64_t *t);


#endif // WIFI_TCP_H
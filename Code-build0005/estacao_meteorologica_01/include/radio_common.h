#ifndef RADIO_COMMON_H
#define RADIO_COMMON_H

#define POST_TIMEOUT_US         5000000 // 5s

#define POST_PATH_SEND_DATA     "/dados"
#define POST_PATH_GET_TIME      "/hora"


typedef enum{
    RADIO_POST_ST_IDLE = 0,
    RADIO_POST_ST_DATA_CONNECTING,
    RADIO_POST_ST_DATA_SENDING,
    RADIO_POST_ST_DATA_SENT_OK,
    RADIO_POST_ST_DATA_SEND_ERROR,
    RADIO_POST_ST_TIME_REQUEST_CONNECTING,
    RADIO_POST_ST_TIME_REQUEST_SENDING,
    RADIO_POST_ST_TIME_REQUEST_RECEIVED_OK,
    RADIO_POST_ST_TIME_REQUEST_ERROR,
} RadioPostState;

typedef struct {
    struct tcp_pcb *pcb;
    char            buffer[512+256];
    int             sent_len;
    int             total_len;
    bool            busy;
    uint64_t        start_post;
    RadioPostState  radio_post_state;
} RadioPostData;

extern RadioPostData radio_post_data;

#endif // RADIO_COMMON_H
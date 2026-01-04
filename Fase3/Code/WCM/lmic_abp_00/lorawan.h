#ifndef LORAWAN_H
#define LORAWAN_H

typedef enum{
  LORAWAN_STATE_SLEEPING = 0,
  LORAWAN_STATE_BUSY
}LorawanState;

typedef enum{
  LORAWAN_MODE_INVALID = 0,
  LORAWAN_MODE_LORA,
  LORAWAN_MODE_ABP,
  LORAWAN_MODE_OTAA
}LorawanMode;

typedef struct{
  LorawanMode mode;
  uint8_t     channel;
  uint8_t     sf;
  uint8_t     fcnt[2];
  uint8_t     devaddr[4];
  uint8_t     appskey[16];
  uint8_t     nwkskey[16];

  uint8_t     msg[UART_BUFFER_SIZE];
  int         msg_size;
}LorawanPars;

extern LorawanPars lorawan_pars;

void         lorawam_setup();
void         lorawam_update_state();
void         lorawan_send();
LorawanState lorawan_state();


#endif // LORAWAN_H
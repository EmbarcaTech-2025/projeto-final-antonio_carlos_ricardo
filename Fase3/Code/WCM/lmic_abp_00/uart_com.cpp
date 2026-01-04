#include <Arduino.h>
#include "config.h"
#include "lorawan.h"
#include "uart_com.h"

HardwareSerial       SerialUART(1);
static char          buffer[UART_BUFFER_SIZE];
static int           buffer_pos;
static unsigned long t_end; 


void      uart_setup(){
  SerialUART.begin(115200, SERIAL_8N1, RX_PIN, TX_PIN);
  lorawan_pars.mode = LORAWAN_MODE_INVALID;
}

void uart_start_read_data(){
  buffer_pos = 0;

  // limpar buffer
  while(Serial.available() > 0){
    Serial.read();
  }
  t_end = millis() + UART_READ_T_MAX_MS;
}

static void printf_buffer(){
  Serial.print("Buffer len = ");
  Serial.print(buffer_pos);
  Serial.print(", CMD: ");
  for(int i=0; i<buffer_pos;i++)Serial.print(buffer[i]);
  Serial.println();
}

static bool read_channel(char *data){
  lorawan_pars.channel = ((data[0] - '0') * 10) + (data[1] - '0');
  if(lorawan_pars.channel < 64){
    Serial.print("  - uard_read_channel: ");
    Serial.println(lorawan_pars.channel);
    return true;  
  }else{
    Serial.print("  - uard_read_channel: INVALID = ");
    Serial.println(lorawan_pars.channel);
    return false;
  }
}

static bool read_sf(char *data){
  lorawan_pars.sf = ((data[0] - '0') * 10) + (data[1] - '0');
  if((lorawan_pars.sf >= 7)&&(lorawan_pars.sf <= 12)){
    Serial.print("  - SF: ");
    Serial.println(lorawan_pars.sf);
    return true;  
  }else{
    Serial.print("  - SF: INVALID = ");
    Serial.println(lorawan_pars.sf);
    return false;
  }
}

static int8_t get_hexa(char c){
  if((c >= 0)&&(c<='9')){
    return c - '0';
  }else if((c >= 'A')&&(c<='F')){
    return c + 10 - 'A';
  }else if((c >= 'a')&&(c<='f')){
    return c + 10 - 'a';
  }else{
    Serial.print("  - INVALID char: ");  
    Serial.println(c);
    return -1;
  }
}

static bool read_array(uint8_t * array, char * data, int data_size, char * name){
  Serial.print("  - ");
  Serial.print(name);
  Serial.print(": ");
  int pos = 0;
  for(int i=0;i < data_size;i++){
    char c1 = data[pos++];
    char c2 = data[pos++];
    Serial.print(c1);
    Serial.print(c2);
    Serial.print(" ");
    int8_t r1 = get_hexa(c1);
    int8_t r2 = get_hexa(c2);
    if((r1<0)||(r2<0)){
      Serial.println();
      return false;
    }else{
      array[i] = (r1 << 4) | r2;
    }
  }
  Serial.println();
  return true;
}

static bool read_msg(char *data, int data_size){
  lorawan_pars.msg_size = data_size;

  Serial.print("  - MSG: ");
  int pos = 0;
  for(int i=0;i < data_size;i++){
    char c1 = data[pos++];
    char c2 = data[pos++];
    Serial.print(c1);
    Serial.print(c2);
    Serial.print(" ");
    int8_t r1 = get_hexa(c1);
    int8_t r2 = get_hexa(c2);
    if((r1<0)||(r2<0)){
      Serial.println();
      return false;
    }else{
      lorawan_pars.msg[i] = (r1 << 4) | r2;
    }
  }
  Serial.println();
  return true;
}

static UartState valida_lora(){
  Serial.println("Lora command validation:");
  // valid size
  if(buffer_pos < 9){
    Serial.println("Command small < 9");
    return UART_ST_DATA_ERROR;
  }
  if(!((buffer_pos - 9) & 0x01)){
    Serial.println("The message is not an even size.");
    return UART_ST_DATA_ERROR;
  }
  if(!read_channel(&buffer[0x02]))                         return UART_ST_DATA_ERROR;
  if(!read_sf(     &buffer[0x05]))                         return UART_ST_DATA_ERROR;
  if(!read_msg(    &buffer[0x08], (buffer_pos - 0x08)>>1)) return UART_ST_DATA_ERROR;

  lorawan_pars.mode = LORAWAN_MODE_LORA;

  return UART_ST_DATA_READY;
}

static UartState valida_lorawan_abp(){
  Serial.println("LoRaWAN ABP command validation:");
  // valid size
  if(buffer_pos < 0x59){
    Serial.println("Command small < 0x59");
    return UART_ST_DATA_ERROR;
  }
  if(!((buffer_pos - 0x59) & 0x01)){
    Serial.println("The message is not an even size.");
    return UART_ST_DATA_ERROR;
  }
  if(!read_channel(&buffer[0x02])) return UART_ST_DATA_ERROR;
  if(!read_sf(     &buffer[0x05])) return UART_ST_DATA_ERROR;
  if(!read_array(lorawan_pars.devaddr, &buffer[0x08],  4, "devaddr"))          return UART_ST_DATA_ERROR;
  if(!read_array(lorawan_pars.fcnt,    &buffer[0x11],  2, "fCnt"))             return UART_ST_DATA_ERROR;
  if(!read_array(lorawan_pars.appskey, &buffer[0x16], 16, "appskey"))          return UART_ST_DATA_ERROR;
  if(!read_array(lorawan_pars.nwkskey, &buffer[0x37], 16, "nwkskey"))          return UART_ST_DATA_ERROR;
  if(!read_msg(                        &buffer[0x58], (buffer_pos - 0x58)>>1)) return UART_ST_DATA_ERROR;

  lorawan_pars.mode = LORAWAN_MODE_ABP;
  //Serial.println("LoRaWAN ABP command BEFORE SEND");
  lorawan_send();
  return UART_ST_DATA_READY;
}

static UartState valida_lorawan_otaa(){
  //uart_last_cmd = UART_CMD_LORAWAN_OTAA;
  return UART_ST_DATA_READY;
}

UartState uart_state(){
  if(millis() > t_end){ // Check timeout
    Serial.print("Data Timeout: ");
    Serial.print("N chars = ");
    Serial.print(buffer_pos);
    buffer[buffer_pos] = 0;
    Serial.print(", CMD = ");
    Serial.println(buffer);
    return UART_ST_TIMEOUT;
  }

  if(SerialUART.available() > 0){ // Read
    char c = SerialUART.read(); 

    if((c == '\n')||(c == '\r')){
      lorawan_pars.mode = LORAWAN_MODE_INVALID;

      // Valida CMD
      printf_buffer();
      if(buffer_pos <= 1){
        Serial.println("Data ERROR, no data");
        return UART_ST_DATA_ERROR;
      }
      if(buffer[1] != ' '){
        Serial.println("Data ERROR, buffer[1] != ' '");
        return UART_ST_DATA_ERROR;
      }
      switch(buffer[0]){
        case 'L':
        case 'l':
          return valida_lora();
        case 'A':
        case 'a':
          return valida_lorawan_abp();
        case 'O':
        case 'o':
          return valida_lorawan_otaa();
        default:
          Serial.print("Data ERROR, Invalid CMD: ");
          Serial.println(buffer[0]);
          return UART_ST_DATA_ERROR;
      }
    }else{
      if(buffer_pos >= (UART_BUFFER_SIZE-1)){
        Serial.println("Data ERROR, to much data");
        return UART_ST_DATA_ERROR;
      }else{
        buffer[buffer_pos] = c;
        buffer_pos++;
      }
    }
  }
  return UART_ST_WAITING_DATA;
}
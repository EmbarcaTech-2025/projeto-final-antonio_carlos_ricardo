#ifndef STORAGE_H
#define STORAGE_H

#include "estation_data.h"

#define STORAGE_CLEAR_BLOCK_SIZE        0x1000      // 4k
#define STORAGE_WRITE_BLOCK_SIZE        0x0100      // 256

#define STORAGE_OFFSET_CONF             0x0100000
#define STORAGE_OFFSET_DATA_FIRST       0x0101000   // = STORAGE_ADDRESS_CONF + 0x1000
#define STORAGE_ADDRESS_DATA_SIZE       0x0100      // 256 bytes
#define STORAGE_ADDRESS_DATA_N_ITEMS    0xFF0       // 4080 = 255 blocos de 4k bytes
                                                    //      multiplos de 16 pois: 4k/256 = 16
                                                    //      STORAGE_CLEAR_BLOCK / STORAGE_ADDRESS_DATA_SIZE


void stored_conf_clear();
void stored_conf_read();
void stored_conf_write();

void    stored_data_clear(int32_t index);
void    stored_data_clear_all();
void    stored_data_read( int32_t index, EstationData * estation_data);
void    stored_data_write(int32_t index, EstationData * estation_data, bool update);
bool    stored_data_get_last_write_index(uint64_t * max_cycle,  int32_t  * index);
int32_t stored_data_get_next_send_index();


#endif // STORAGE_H
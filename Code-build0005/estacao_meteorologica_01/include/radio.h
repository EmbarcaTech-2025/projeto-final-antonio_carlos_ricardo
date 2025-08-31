#ifndef RADIO_H
#define RADIO_H

void radio_init();
bool radio_update();

bool radio_connected();

bool radio_get_time_r(uint64_t * t);
//bool radio_get_time(uint64_t * t);
bool radio_send_est_data(char * json_msg);
//bool radio_send_data(EstationData * estation_data);


#endif // RADIO_H
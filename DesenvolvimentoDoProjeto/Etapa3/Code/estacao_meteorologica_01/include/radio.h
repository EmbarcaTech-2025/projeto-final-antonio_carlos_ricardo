#ifndef RADIO_H
#define RADIO_H

void radio_init();
void radio_update();

bool radio_get_time(uint64_t * t);
bool radio_send_data(EstationData * estation_data);


#endif // RADIO_H
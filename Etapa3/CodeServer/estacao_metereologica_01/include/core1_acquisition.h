#ifndef CORE1_ACQUISITION_H
#define CORE1_ACQUISITION_H

void core1_acquisition_init(uint64_t cycle);
void core1_acquisition_get();
void core1_acquisition_save();

int32_t core1_acquisition_get_next_send_index();
void    core1_acquisition_sent_last_index();


#endif // CORE1_ACQUISITION_H
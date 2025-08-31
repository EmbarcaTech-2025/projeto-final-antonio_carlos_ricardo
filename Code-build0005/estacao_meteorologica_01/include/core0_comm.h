#ifndef CORE0_COMM_H
#define CORE0_COMM_H

CoreCommunicationCmd core0_comm_aguarda_cmd(void);
void core0_comm_core0_fifo_irq_handler(void);
void core0_comm_new_time(uint64_t new_time);
bool core0_comm_give_last_data();
bool core0_comm_give_next_data();
void core0_comm_give_next_data_sent();


#endif // CORE0_COMM_H
#ifndef AQ_DATA_CPU_TEMP_H
#define AQ_DATA_CPU_TEMP_H

int aqdatacpu_temp_power_on();
int aqdatacpu_temp_init_aq();
int aqdatacpu_temp_read(int *temp);
int aqdatacpu_temp_sleep();

#endif // AQ_DATA_CPU_TEMP_H
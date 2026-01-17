#include "../include/aq_data_lux.h"
#include "../include/bh1750.h"

int aqdatalux_init_power_on(){
    return 0;
}
int aqdatalux_init_aq(){
    bh1750_init();
    return 0;
}
int aqdatalux_read(AqDataLux_Value *value){
    value->lux_level = bh1750_read_lux();
    return 0;
}

int aqdatalux_sleep(){
    return 0;
}
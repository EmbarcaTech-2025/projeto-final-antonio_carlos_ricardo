#include "../include/aq_data_lux.h"

int aqdatalux_init(){
    return 0;
}
int aqdatalux_read(AqDataLux_Value *value){
    value->lux_level = 0xFFFF;
    return 0;
}

int aqdatalux_sleep(){
    return 0;
}
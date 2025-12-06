#include "../include/flash.h"
#include "../include/est_config.h"

void flash_clear_all(){

}
bool flash_conf_read( EstConfig * est_config){
    est_config_default(est_config);
    //return true;
    return false;
}
void flash_conf_write(EstConfig * est_config){

}
#ifndef FLASH_H
#define FLASH_H

#include "est_config.h"


void flash_clear_all();
bool flash_conf_read( EstConfig * est_config);  // false ==> error, true ==> OK
void flash_conf_write(EstConfig * est_config);


#endif // FLASH_H
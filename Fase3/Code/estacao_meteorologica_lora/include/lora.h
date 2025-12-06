#ifndef LORA_H
#define LORA_H

#include "aq_data.h"
#include "est_config.h"

void lora_send(EstConfig *est_config, AqData *aq_data);

#endif // LORA_H
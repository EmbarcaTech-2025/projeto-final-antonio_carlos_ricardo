#ifndef WCM_H
#define WCM_H

#include "aq_data.h"
#include "est_config.h"




void wcm_init();
void wcm_send(EstConfig *est_config, AqData *aq_data);

#endif // WCM_H
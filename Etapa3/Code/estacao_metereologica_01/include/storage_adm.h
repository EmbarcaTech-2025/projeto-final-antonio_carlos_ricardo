#ifndef STORAGE_ADM_H
#define STORAGE_ADM_H

uint64_t storage_adm_init();
int32_t  storage_adm_get_next_index();
int32_t  storage_adm_get_next_sent();
int32_t  storage_adm_can_update_lest_send();


#endif // STORAGE_ADM_H
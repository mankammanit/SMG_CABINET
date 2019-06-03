#ifndef __nvs_storage_H__
#define __nvs_storage_H__

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "nvs.h"

#include "sdkconfig.h"
#include "st_profile.h"


void save_ratio(ratio_val ptr);
bool read_ratio(ratio_val *ptr);
void save_ph_kvalue(ph_str_val ptr);
bool read_ph_kvalue(ph_str_val *ptr);


#endif

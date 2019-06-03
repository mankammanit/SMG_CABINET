#ifndef __TOOLS_CM_H__
#define __TOOLS_CM_H__

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "main.h"
#include "st_profile.h"
#include "nvs_storage.h"
#include "tools_cm.h"


long map(long x);
void fade1(uint8_t val , uint8_t val2 , uint8_t val3 , uint8_t val4);

#endif

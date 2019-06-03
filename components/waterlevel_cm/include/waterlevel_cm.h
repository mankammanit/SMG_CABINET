#ifndef __WATERLEVEL_CM_H__
#define __WATERLEVEL_CM_H__

#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"
#include "esp_log.h"
#include <time.h>

void waterlevel_init1();
uint16_t read_water_lv1();
void waterlevel_init2();
uint16_t read_water_lv2();

#endif

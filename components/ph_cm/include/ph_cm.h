#ifndef __PH_CM_H__
#define __PH_CM_H__

#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"
#include "esp_log.h"
#include <time.h>

#define PH_CH  ADC2_CHANNEL_7
static const adc_atten_t atten = ADC_ATTEN_DB_11;
int read_raw;
int adc_reading_ph;
float readPH(float voltage);
void init_ph();
void phCalibration(uint8_t mode);
void calibration(float voltage,uint8_t mode);
//define value

#endif

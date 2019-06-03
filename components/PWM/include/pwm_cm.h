#ifndef __PWM_CM_H__
#define __PWM_CM_H__

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"

//fan inside & intake
#define FAN_INTAKE   25
#define FAN_INSIDE   26

void pwm_cm(uint8_t ch , uint8_t duty , int freq);
void init_faninside();
void init_fanintake();

#endif

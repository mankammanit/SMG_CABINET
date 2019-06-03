
#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "driver/dac.h"
#include "esp_system.h"
#include "esp_adc_cal.h"
#include "string.h"
#include "waterlevel_cm.h"

#define WATERLEVEL_CH  ADC1_CHANNEL_6
#define WATERLEVEL2_CH  ADC1_CHANNEL_7
static const adc_atten_t atten = ADC_ATTEN_DB_0;
static const adc_unit_t unit = ADC_UNIT_1;


static void check_efuse()
{
  //Check TP is burned into eFuse
  if (esp_adc_cal_check_efuse(ESP_ADC_CAL_VAL_EFUSE_TP) == ESP_OK)
  {
    printf("eFuse Two Point: Supported\n");
  }
  else
  {
    printf("eFuse Two Point: NOT supported\n");
  }

  //Check Vref is burned into eFuse
  if (esp_adc_cal_check_efuse(ESP_ADC_CAL_VAL_EFUSE_VREF) == ESP_OK)
  {
    printf("eFuse Vref: Supported\n");
  }
  else
  {
    printf("eFuse Vref: NOT supported\n");
  }
}

uint16_t read_water_lv1()
{
  uint16_t adc_reading_waterlv = 0;
  adc_reading_waterlv += adc1_get_raw((adc1_channel_t)WATERLEVEL_CH);
  return adc_reading_waterlv;
}

uint16_t read_water_lv2()
{
  uint16_t adc_reading_waterlv2 = 0;
  adc_reading_waterlv2 += adc1_get_raw((adc1_channel_t)WATERLEVEL2_CH);
  return adc_reading_waterlv2;
}


void waterlevel_init1()
{
  check_efuse();
  adc1_config_width(ADC_WIDTH_BIT_12);
  adc1_config_channel_atten(WATERLEVEL_CH, atten);
}

void waterlevel_init2()
{
  check_efuse();
  adc1_config_width(ADC_WIDTH_BIT_12);
  adc1_config_channel_atten(WATERLEVEL2_CH, atten);
}

/* ADC2 Example
This example code is in the Public Domain (or CC0 licensed, at your option.)
Unless required by applicable law or agreed to in writing, this
software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied.
*/
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
#include "st_profile.h"
#include "nvs_storage.h"
#include "uart_cm.h"
#include "ph_cm.h"



void ph_add_val()
{
  ph_val._temp_c = 25.0;
  ph_val._phValue = 7.0;
  ph_val._acidVoltage = 185.0;    //buffer solution 4.0 at 25C
  ph_val._neutralVoltage = 160.0; //buffer solution 7.0 at 25C
  ph_val._voltage = 160.0;
  printf("ph_addval\n");
}

void calibration(float voltage,uint8_t mode)
{
  ph_val._voltage = voltage;
  ph_val._temp_c = 25.0;
  phCalibration(mode);
  // printf("calibration : %.1f , %.1f \n",ph_val._voltage,ph_val._temp_c);
}


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

void phCalibration(uint8_t mode)
{
  static bool phCalibrationFinish = 0;
  static bool enterCalibrationFlag = 0;
  switch (mode)
  {
    case 0:
    if (enterCalibrationFlag)
    {
      printf(">>>pH Command Error<<<\n");
    }
    break;

    case 1:
    enterCalibrationFlag = 1;
    phCalibrationFinish = 0;

    printf(">>>Enter PH Calibration Mode<<<\n");
    printf(">>>Please put the probe into the 4.0 or 7.0 standard buffer solution<<<\n");

    break;


    case 2:

    if(enterCalibrationFlag){

      if((ph_val._voltage>155)&&(ph_val._voltage<165))
      // buffer solution:7.0
      {
        printf(">>>pH Buffer Solution:7.0\n");
        ph_val._neutralVoltage = ph_val._voltage;
        printf(",Send EXITPH to Save and Exit<<<\n");
        phCalibrationFinish = 1;
      }
      else if((ph_val._voltage>180)&&(ph_val._voltage<190))
      {  //buffer solution:4.0
        printf(">>>pH Buffer Solution:4.0\n");
        ph_val._acidVoltage = ph_val._voltage;
        printf(",Send EXITPH to Save and Exit<<<\n");
        phCalibrationFinish = 1;
      }
      else
      {
        printf(">>>pH Buffer Solution Error Try Again<<<\n");
        // not buffer solution or faulty operation
        phCalibrationFinish = 0;
      }
    }
    break;

    case 3:
    if (enterCalibrationFlag)
    {


      if(phCalibrationFinish)
      {
        if((ph_val._voltage>155)&&(ph_val._voltage<165))
        {
          save_ph_kvalue(ph_val);
          printf("debug ph nvs_save1\n");
        }
        else if((ph_val._voltage>180)&&(ph_val._voltage<190))
        {
          save_ph_kvalue(ph_val);
          printf("debug ph nvs_save2\n");
        }
        printf(">>>pH Calibration Successful\n");
      }
      else
      {
        printf(">>>pH Calibration Failed\n");
      }
      printf(",Exit PH Calibration Mode<<<\n");

      phCalibrationFinish = 0;
      enterCalibrationFlag = 0;
    }
    break;
  }
}

float readPH(float voltage)
{
  float slope = (7.0-4.0)/((ph_val._neutralVoltage-160.0)/3.0 - (ph_val._acidVoltage-160.0)/3.0);  // two point: (_neutralVoltage,7.0),(_acidVoltage,4.0)
  float intercept =  7.0 - slope*(ph_val._neutralVoltage-160.0)/3.0;

  ph_val._phValue = slope*(voltage-160.0)/3.0+intercept;  //y = k*x + b
  return ph_val._phValue;
}


void init_ph()
{
  ph_add_val();
  check_efuse();
  adc_reading_ph=0;
  adc2_config_channel_atten(PH_CH, atten );
  esp_err_t status = adc2_vref_to_gpio(GPIO_NUM_27);
if (status == ESP_OK) {
    printf("v_ref routed to GPIO\n");
} else {
    printf("failed to route v_ref\n");
}
}


//debug check init
// esp_err_t r =adc2_get_raw( GPIO_PH, ADC_WIDTH_12Bit, &read_raw);
//   if ( r == ESP_OK ) {
//     printf("READ VOLT : %d\n", read_raw );
//   }
//   else if ( r == ESP_ERR_INVALID_STATE ) {
//     printf("ADC2 not initialized yet.\n");
//   }
//  else if ( r == ESP_ERR_TIMEOUT ) {
//   printf("ADC2 used by Wi-Fi.\n");
// }

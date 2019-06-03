#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


#include "main.h"
#include "st_profile.h"
#include "nvs_storage.h"
#include "tools_cm.h"
#include "pca9685.h"
#include "string.h"

i2c_dev_t dev;
//function map kammanit 0-4096
long map(long x)
{
  x=100-x; //invert 100 is 0
  printf("x: %lu\n",x);
  long cal = (x - 0) * (4080 - 0) / (100 - 0) + 0;
  printf("cal: %lu\n",cal);
  return cal;
}

//function fade led i2c kammanit
void fade1(uint8_t val , uint8_t val2 , uint8_t val3 , uint8_t val4)
{

  val = ratio_123.ratio1_cp;
  val2 = ratio_123.ratio2_cp;
  val3 = ratio_123.ratio3_cp;

  val_ratio1 = (dtmp[1]*dtmp[4])/100;
  val_ratio2 = (dtmp[2]*dtmp[4])/100;
  val_ratio3 = (dtmp[3]*dtmp[4])/100;

  printf("val_ratio1: %d\n",val_ratio1);
  printf("val_ratio2: %d\n",val_ratio2);
  printf("val_ratio3: %d\n",val_ratio3);

  val_ratio1 = (dtmp[1]*dtmp[4])/100;
  val_ratio2 = (dtmp[2]*dtmp[4])/100;
  val_ratio3 = (dtmp[3]*dtmp[4])/100;

  if(val==val_ratio1) pca9685_set_pwm_value(&dev, LED_CH1, map(ratio_123.ratio1));
  if(val2==val_ratio2)pca9685_set_pwm_value(&dev, LED_CH2, map(ratio_123.ratio2));
  if(val3==val_ratio3)pca9685_set_pwm_value(&dev, LED_CH3, map(ratio_123.ratio3));

  if(val!=val_ratio1)
  {
    if(val>val_ratio1)
    {
      printf("ration1 = %d > val_ratio1 = %d\n",val,val_ratio1);
      if(val<=100 && val>=60 && val%2==0 && val_ratio1<=30)
      {
        for(int i = val;i>=val_ratio1;i=i-2)
        {
          printf("ratio loop : %d\n",i);
          pca9685_set_pwm_value(&dev, LED_CH1, map(i));
          vTaskDelay(5 / portTICK_PERIOD_MS);
        }
      }
      else if(val<=100 && val>=60 && val%2!=0 && val_ratio1<=30)
      {
        for(int i = val+1;i>=val_ratio1;i=i-2)
        {
          printf("ratio loop : %d\n",i);
          pca9685_set_pwm_value(&dev, LED_CH1, map(i));
          vTaskDelay(5 / portTICK_PERIOD_MS);
        }
      }
      else
      {
        for(int i = val;i>=val_ratio1;i--)
        {
          printf("ratio loop : %d\n",i);
          pca9685_set_pwm_value(&dev, LED_CH1, map(i));
          vTaskDelay(5 / portTICK_PERIOD_MS);
        }
      }
      ratio_123.ratio1_cp = val_ratio1;
    }
    else if(val<val_ratio1)
    {
      printf("ration1 = %d < val_ratio1 = %d\n",val,val_ratio1);
      if(val<=60 && val%2==0 && val_ratio1>=80)
      {
        for(int i =val;i<=val_ratio1;i=i+2)
        {
          printf("ratio loop : %d\n",i);
          pca9685_set_pwm_value(&dev, LED_CH1, map(i));
          vTaskDelay(5 / portTICK_PERIOD_MS);
        }
      }
      else if(val<=60 && val%2==1 && val_ratio1>=80)
      {
        for(int i =val+1;i<=val_ratio1+1;i=i+2)
        {
          printf("ratio loop : %d\n",i);
          pca9685_set_pwm_value(&dev, LED_CH1, map(i));
          vTaskDelay(5 / portTICK_PERIOD_MS);
        }
      }
      else
      {
        for(int i =val;i<=val_ratio1;i++)
        {
          printf("ratio loop : %d\n",i);
          pca9685_set_pwm_value(&dev, LED_CH1, map(i));
          vTaskDelay(5 / portTICK_PERIOD_MS);
        }
      }
      ratio_123.ratio1_cp = val_ratio1;
    }
  }

  if(val2!=val_ratio2)
  {
    if(val2>val_ratio2)
    {
      printf("ration2 = %d > val_ratio2 = %d\n",val2,val_ratio2);
      if(val2<=100 && val2>=60 && val2%2==0 && val_ratio2<=30)
      {
        for(int i=val2;i>=val_ratio2;i=i-2)
        {
          printf("ratio loop : %d\n",i);
          pca9685_set_pwm_value(&dev, LED_CH2, map(i));
          vTaskDelay(5 / portTICK_PERIOD_MS);
        }
      }
      else if(val2<=100 && val2>=60 && val2%2!=0 && val_ratio2<=30)
      {
        for(int i=val2+1;i>=val_ratio2;i=i-2)
        {
          printf("ratio loop : %d\n",i);
          pca9685_set_pwm_value(&dev, LED_CH2, map(i));
          vTaskDelay(5 / portTICK_PERIOD_MS);
        }
      }
      else
      {
        for(int i=val2;i>=val_ratio2;i--)
        {
          printf("ratio loop : %d\n",i);
          pca9685_set_pwm_value(&dev, LED_CH2, map(i));
          vTaskDelay(5 / portTICK_PERIOD_MS);
        }
      }
      ratio_123.ratio2_cp = val_ratio2;
    }
    else if(val2<val_ratio2)
    {
      printf("ration2 = %d < val_ratio2 = %d\n",val2,val_ratio2);
      if(val2<=60 && val2%2==0 && val_ratio2>=80)
      {
        for(int i=val2;i<=val_ratio2;i=i+2)
        {
          printf("ratio loop : %d\n",val2);
          pca9685_set_pwm_value(&dev, LED_CH2, map(i));
          vTaskDelay(5 / portTICK_PERIOD_MS);
        }
      }
      else if(val2<=60 && val2%2==1 && val_ratio2>=80)
      {
        for(int i=val2+1;i<=val_ratio2;i=i+2)
        {
          printf("ratio loop : %d\n",val2);
          pca9685_set_pwm_value(&dev, LED_CH2, map(i));
          vTaskDelay(5 / portTICK_PERIOD_MS);
        }
      }
      else
      {
        for(int i=val2;i<=val_ratio2;i++)
        {
          printf("ratio loop : %d\n",val2);
          pca9685_set_pwm_value(&dev, LED_CH2, map(i));
          vTaskDelay(5 / portTICK_PERIOD_MS);
        }
      }
      ratio_123.ratio2_cp = val_ratio2;
    }
  }

  if(val3!=val_ratio3)
  {
    if(val3>val_ratio3)
    {
      printf("ration3 = %d > val_ratio3 = %d\n",val3,val_ratio3);
      if(val3<=100 && val3>=60 && val3%2==0 && val_ratio3<=30)
      {
        for(int i=val3;i>=val_ratio3;i=i-2)
        {
          printf("ratio loop : %d\n",i);
          pca9685_set_pwm_value(&dev, LED_CH3, map(i));
          vTaskDelay(5 / portTICK_PERIOD_MS);
        }
      }
      else if(val3<=100 && val3>=60 && val3%2!=0 && val_ratio3<=30)
      {
        for(int i=val3+1;i>=val_ratio3;i=i-2)
        {
          printf("ratio loop : %d\n",i);
          pca9685_set_pwm_value(&dev, LED_CH3, map(i));
          vTaskDelay(5 / portTICK_PERIOD_MS);
        }
      }
      else
      {
        for(int i=val3;i>=val_ratio3;i--)
        {
          printf("ratio loop : %d\n",i);
          pca9685_set_pwm_value(&dev, LED_CH3, map(i));
          vTaskDelay(5 / portTICK_PERIOD_MS);
        }
      }
      ratio_123.ratio3_cp = val_ratio3;
    }
    else if(val3<val_ratio3)
    {
      printf("ration3 = %d < val_ratio3 = %d\n",val3,val_ratio3);
      if(val2<=60 && val2%2==0 && val_ratio2>=80)
      {
        for(int i=val3;i<=val_ratio3;i=i+2)
        {
          printf("ratio loop : %d\n",i);
          pca9685_set_pwm_value(&dev, LED_CH3, map(i));
          vTaskDelay(5 / portTICK_PERIOD_MS);
        }
      }
      else if(val3<=60 && val3%2==1 && val_ratio3>=80)
      {
        for(int i=val3+1;i<=val_ratio3;i=i+2)
        {
          printf("ratio loop : %d\n",i);
          pca9685_set_pwm_value(&dev, LED_CH3, map(i));
          vTaskDelay(5 / portTICK_PERIOD_MS);
        }
      }
      else
      {
        for(int i=val3;i<=val_ratio3;i++)
        {
          printf("ratio loop : %d\n",i);
          pca9685_set_pwm_value(&dev, LED_CH3, map(i));
          vTaskDelay(5 / portTICK_PERIOD_MS);
        }
      }
      ratio_123.ratio3_cp = val_ratio3;
    }
  }

  if(val4!=val_ratio3)
  {
    if(val4>val_ratio3)
    {
      printf("ration4 = %d > val_ratio4 = %d\n",val4,val_ratio3);
      if(val4<=100 && val4>=60 && val4%2==0 && val_ratio3<=30)
      {
        for(int i=val4;i>=val_ratio3;i=i-2)
        {
          printf("ratio loop : %d\n",i);
          pca9685_set_pwm_value(&dev, LED_CH4, map(i));
          vTaskDelay(5 / portTICK_PERIOD_MS);
        }
      }
      else if(val4<=100 && val4>=60 && val4%2!=0 && val_ratio3<=30)
      {
        for(int i=val4+1;i>=val_ratio3;i=i-2)
        {
          printf("ratio loop : %d\n",i);
          pca9685_set_pwm_value(&dev, LED_CH4, map(i));
          vTaskDelay(5 / portTICK_PERIOD_MS);
        }
      }
      else
      {
        for(int i=val4;i>=val_ratio3;i--)
        {
          printf("ratio loop : %d\n",i);
          pca9685_set_pwm_value(&dev, LED_CH4, map(i));
          vTaskDelay(5 / portTICK_PERIOD_MS);
        }
      }
      ratio_123.ratio3_cp = val_ratio3;
    }
    else if(val4<val_ratio3)
    {
      printf("ration4 = %d < val_ratio4 = %d\n",val4,val_ratio3);
      if(val4<=60 && val4%2==0 && val_ratio3>=80)
      {
        for(int i=val4;i<=val_ratio3;i=i+2)
        {
          printf("ratio loop : %d\n",i);
          pca9685_set_pwm_value(&dev, LED_CH4, map(i));
          vTaskDelay(5 / portTICK_PERIOD_MS);
        }
      }
      else if(val4<=60 && val4%2==1 && val_ratio3>=80)
      {
        for(int i=val3+1;i<=val_ratio3;i=i+2)
        {
          printf("ratio loop : %d\n",i);
          pca9685_set_pwm_value(&dev, LED_CH4, map(i));
          vTaskDelay(5 / portTICK_PERIOD_MS);
        }
      }
      else
      {
        for(int i=val4;i<=val_ratio3;i++)
        {
          printf("ratio loop : %d\n",i);
          pca9685_set_pwm_value(&dev, LED_CH4, map(i));
          vTaskDelay(5 / portTICK_PERIOD_MS);
        }
      }
      ratio_123.ratio3_cp = val_ratio3;
    }
  }
}

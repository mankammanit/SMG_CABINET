#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"

//fan inside & intake
#define FAN_INTAKE   25
#define FAN_INSIDE   26

int32_t freq;
uint8_t duty;

void init_fanintake()
{
  freq = 10;
  duty = 0;
  ledc_timer_config_t ledc_timer =
      {
        .speed_mode = LEDC_HIGH_SPEED_MODE,     // LEDC speed speed_mode, high-speed mode or low-speed mode
        .timer_num = LEDC_TIMER_0,              // The timer source of channel (0 - 3)
        .duty_resolution = LEDC_TIMER_8_BIT,    // range 0..255
        .freq_hz = freq,                        // changes/second
      };
  ledc_timer_config(&ledc_timer);
  ledc_channel_config_t ledc_channel =
      {
        .timer_sel = LEDC_TIMER_0,
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .channel = LEDC_CHANNEL_0,
        .gpio_num = FAN_INTAKE,
        .duty = duty
      };
  ledc_channel_config(&ledc_channel);
  ledc_fade_func_install(0);
}

void init_faninside()
{
  freq = 10;
  duty = 0;
  ledc_timer_config_t ledc_timer =
      {
        .speed_mode = LEDC_HIGH_SPEED_MODE,     // LEDC speed speed_mode, high-speed mode or low-speed mode
        .timer_num = LEDC_TIMER_0,              // The timer source of channel (0 - 3)
        .duty_resolution = LEDC_TIMER_8_BIT,    // range 0..255
        .freq_hz = freq,                        // changes/second
      };
  ledc_timer_config(&ledc_timer);
  ledc_channel_config_t ledc_channel =
      {
        .timer_sel = LEDC_TIMER_0,
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .channel = LEDC_CHANNEL_1,
        .gpio_num = FAN_INSIDE,
        .duty = duty
      };
  ledc_channel_config(&ledc_channel);
  ledc_fade_func_install(0);
}



void pwm_cm(uint8_t ch , uint8_t duty , int freq)
{

if(ch==25)
{
  ledc_set_freq(LEDC_HIGH_SPEED_MODE, LEDC_TIMER_0, freq);
  ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, duty);
  ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0);
}
else if(ch==26)
{
  ledc_set_freq(LEDC_HIGH_SPEED_MODE, LEDC_TIMER_0, freq);
  ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_1, duty);
  ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_1);
}


// ledc_set_freq(LEDC_HIGH_SPEED_MODE, LEDC_TIMER_0, freq);
// ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, duty);
// ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0);
// ledc_set_fade_with_time(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, 255, 4 * 1000);
// ledc_fade_start(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, LEDC_FADE_NO_WAIT); // LEDC_FADE_NO_WAIT LEDC_FADE_WAIT_DONE
}

#include <stdio.h>
#include <stdlib.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "esp_system.h"
#include "esp_log.h"
#include "esp_heap_caps.h"
#include "string.h"
#include "sdkconfig.h"


//cm include lib
#include "main.h"
#include "st_profile.h"
#include "nvs_storage.h"
#include "DHT22.h"
#include "buzzer.h"
#include "pca9685.h"
//kammanit modify lib
#include "uart_cm.h"
#include "ph_cm.h"
#include "waterlevel_cm.h"
#include "pwm_cm.h"
#include "tools_cm.h"

i2c_dev_t dev;
static char tag[] = "i2cscanner";


void open_ciller(uint8_t gpioPin) {
  // Turn on for brief time, then turn off
  gpio_set_level(gpioPin, 1);
  vTaskDelay(500 / portTICK_PERIOD_MS);
  gpio_set_level(gpioPin, 0);
}

void TFT_RESTART()
{
  sprintf(str_name,TFT_REST);
  send_tft(str_name);
}

void DHT_task(void *pvParameter)
{

  printf( "Starting DHT Task\n\n");

  while(1) {
    char temp_dht[5];
    printf("=== Reading DHT ===\n" );

    setDHTgpio(dht22_gpio);
    int ret = readDHT();
    errorHandler(ret);

    printf( "Hum %.1f\t", getHumidity() );
    printf( "Tmp %.1f\n", getTemperature() );


    // send tft
    sprintf(temp_dht, "%.1f", getTemperature());
    sprintf(str_name,TEMP_SEND,temp_dht);
    send_tft(str_name);

    vTaskDelay( 100 / portTICK_RATE_MS );

    setDHTgpio(dht22_gpio2);
    int ret2 = readDHT();
    errorHandler(ret2);

    printf( "Hum2 %.1f\t", getHumidity() );
    printf( "Tmp2 %.1f\n", getTemperature() );


    // send tft
    sprintf(temp_dht, "%.1f", getTemperature());
    sprintf(str_name,TEMP_SEND2,temp_dht);
    send_tft(str_name);

    vTaskDelay( 100 / portTICK_RATE_MS );


    setDHTgpio(dht22_gpio3);
    int ret3 = readDHT();
    errorHandler(ret3);

    printf( "Hum3 %.1f\t", getHumidity() );
    printf( "Tmp3 %.1f\n", getTemperature() );

    // send tft
    sprintf(temp_dht, "%.1f", getTemperature());
    sprintf(str_name,TEMP_SEND3,temp_dht);
    send_tft(str_name);

    // open_ciller(RELAY_GPIO);
    ///////////testfan//////////
    for(int i=0;i<=255;i++)
    {
      // printf("loop: %d\n",i);
      pwm_cm(FAN_INTAKE , i , 5000);
      pwm_cm(FAN_INSIDE , i , 5000);
      vTaskDelay( 10 / portTICK_RATE_MS );
    }
    pwm_cm(FAN_INTAKE , 0 , 5000);
    pwm_cm(FAN_INSIDE , 0 , 5000);
    ///////////////////////////

    // -- wait at least 10 sec before reading again ------------
    // The interval of whole process must be beyond 10 seconds !!
    vTaskDelay( 10000 / portTICK_RATE_MS );
  }
}


void waterlv(void *pvParameter)
{

  while(1) {
    //waterlv1
    if(read_water_lv1()==4095)
    {
      printf("Detect Water LV1\n");
    }
    else{
      printf("No detect Water LV1\n");
    }
    //waterlv2
    if(read_water_lv2()==4095)
    {
      printf("Detect Water LV2\n");
    }
    else{
      printf("No detect Water LV2\n");
    }

    vTaskDelay( 1000 / portTICK_RATE_MS );
  }
}


void scani2c()
{

  ESP_LOGD(tag, ">> i2cScanner");
  i2c_config_t conf;
  conf.mode = I2C_MODE_MASTER;
  conf.sda_io_num = SDA_GPIO;
  conf.scl_io_num = SCL_GPIO;
  conf.sda_pullup_en = GPIO_PULLUP_DISABLE;
  conf.scl_pullup_en = GPIO_PULLUP_DISABLE;
  conf.master.clk_speed = 400000;
  i2c_param_config(I2C_NUM_0, &conf);

  i2c_driver_install(I2C_NUM_0, I2C_MODE_MASTER, 0, 0, 0);

  int i;
  esp_err_t espRc;
  printf("Starting scan\n");
  for (i=3; i< 0x78; i++) {
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (i << 1) | I2C_MASTER_WRITE, 1 /* expect ack */);
    i2c_master_stop(cmd);

    espRc = i2c_master_cmd_begin(I2C_NUM_0, cmd, 100/portTICK_PERIOD_MS);
    if (espRc == 0) {
      printf("Found device at ID 0x%.2x\n", i);
    }
    i2c_cmd_link_delete(cmd);
  }
  printf("Scan complete \n");
}

void initpca9685()
{

  i2cdev_init();

  pca9685_init_desc(&dev, ADDR, 0, SDA_GPIO, SCL_GPIO);

  pca9685_init(&dev);

  pca9685_restart(&dev);

  pca9685_set_pwm_frequency(&dev, 1000);

  uint16_t freq;

  pca9685_get_pwm_frequency(&dev, &freq);

  printf("Freq 1000Hz, real %d\n", freq);

}

static void RECV_CALL_TFT()
{

  while(1)
  {

    strcpy(dtmp, readtft());
    // printf("strlen %s\n",dtmp);
    for (uint8_t i = 0; i <  strlen(dtmp); i++)
    {
      // printf("data rev[%d] %02x\n", i, dtmp[i]);
      if(dtmp[1]==255)dtmp[1]=0;
      if(dtmp[2]==255)dtmp[2]=0;
      if(dtmp[3]==255)dtmp[3]=0;
      if(dtmp[4]==255)dtmp[4]=0;
      printf("dtmp[%d] %d\n", i, ((uint8_t)dtmp[i]));
      // printf("dtmp = %d\n", dtmp[i]);
    }

    switch (dtmp[0])
    {

      case PAGE_RATIO:

      printf("case PAGE_RATIO");
      read_ratio(&ratio_123);


      printf("readnvs1 ratio1 : %d\n",ratio_123.ratio1);
      printf("readnvs2 ratio2 : %d\n",ratio_123.ratio2);
      printf("readnvs3 ratio3 : %d\n",ratio_123.ratio3);
      printf("readnvs4 brightness : %d\n",ratio_123.brightness);


      sprintf(str_name,SET_RATIO_1,ratio_123.ratio1);
      send_tft(str_name);

      sprintf(str_name,SET_RATIO_2,ratio_123.ratio2);
      send_tft(str_name);

      sprintf(str_name,SET_RATIO_3,ratio_123.ratio3);
      send_tft(str_name);

      sprintf(str_name,SET_BRIGHTNESS,ratio_123.brightness);
      send_tft(str_name);

      ratio_123.ratio1 = (ratio_123.ratio1*ratio_123.brightness)/100;
      ratio_123.ratio2 = (ratio_123.ratio2*ratio_123.brightness)/100;
      ratio_123.ratio3 = (ratio_123.ratio3*ratio_123.brightness)/100;

      CHECK_LOOP(pca9685_set_pwm_value(&dev, LED_CH1, map(ratio_123.ratio1)),
      "Could not set PWM value to ch15");
      CHECK_LOOP(pca9685_set_pwm_value(&dev, LED_CH2, map(ratio_123.ratio2)),
      "Could not set PWM value to ch14");
      CHECK_LOOP(pca9685_set_pwm_value(&dev, LED_CH3, map(ratio_123.ratio3)),
      "Could not set PWM value to ch13");



      break;

      case RATIO:
      printf("case RATIO\n");
      //nvs read check
      if(read_ratio(&ratio_123))
      {

        fade1(ratio_123.ratio1,ratio_123.ratio2,ratio_123.ratio3,ratio_123.ratio3);

        // bz_tone(Eb8,600);
        //nvs save
        ratio_123.ratio1 = dtmp[1];
        ratio_123.ratio2 = dtmp[2];
        ratio_123.ratio3 = dtmp[3];
        save_ratio(ratio_123);

      }

      break;

      case BRIGHTNESS:
      printf("case BRIGHTNESS\n");

      //read nvs
      read_ratio(&ratio_123);

      uint8_t bright1 = (dtmp[1]*dtmp[4])/100;
      uint8_t bright2 = (dtmp[2]*dtmp[4])/100;
      uint8_t bright3 = (dtmp[3]*dtmp[4])/100;

      printf("bright(main): %d\n",dtmp[4]);
      printf("bright1: %d\n",bright1);
      printf("bright2: %d\n",bright2);
      printf("bright3: %d\n",bright3);

      ratio_123.brightness = dtmp[4];

      pca9685_set_pwm_value(&dev, LED_CH1, map(bright1));
      pca9685_set_pwm_value(&dev, LED_CH2, map(bright2));
      pca9685_set_pwm_value(&dev, LED_CH3, map(bright3));
      pca9685_set_pwm_value(&dev, LED_CH4, map(bright3));
      // CHECK_LOOP(pca9685_set_pwm_value(&dev, LED_CH3, map(bright3)),"LED_CH3 ERROR");

      //nvs save
      vTaskDelay( 10 / portTICK_RATE_MS );
      // bz_tone(Eb8,600);
      save_ratio(ratio_123);


      break;

      case ERASE_FLASH:
      printf("case ERASE_FLASH\n");
      bz_tone(Eb8,600);
      nvs_flash_erase();
      esp_restart();
      break;

      case CALIBRAT_EC:
      printf("case CALIBRAT_EC\n");
      if(dtmp[1]==1)
      {
        printf("case CALIBRAT_EC mode1\n");
        send_ec("A");
      }
      else if(dtmp[1]==2)
      {
        printf("case CALIBRAT_EC mode2\n");
        send_ec("B");
      }
      if(dtmp[1]==3)
      {
        printf("case CALIBRAT_EC mode3\n");
        send_ec("C");
      }

      break;

      case CALIBRAT_PH:
      printf("case CALIBRAT_EC\n");
      if(dtmp[1]==1)
      {
        printf("case CALIBRAT_PH mode1\n");
        calibration(adc_reading_ph,dtmp[1]);
      }
      else if(dtmp[1]==2)
      {
        printf("case CALIBRAT_PH mode2\n");
        calibration(adc_reading_ph,dtmp[1]);
      }
      if(dtmp[1]==3)
      {
        printf("case CALIBRAT_PH mode3\n");
        calibration(adc_reading_ph,dtmp[1]);
      }
      break;


    }
  }
}

void RECV_CALL_EC(void *pvParameter)
{
  while(1)
  {

    static char ec_val[5];
    strcpy(ec_val, readec());
    // printf("strlen %s\n",dtmp);

    // for (uint8_t i = 0; i <  strlen(ec_val); i++)
    // {
    //   // printf("data rev[%d] %02x\n", i, ec_val[i]);
    //   printf("ec_val[%d] %d\n", i, ((uint8_t)ec_val[i]));
    // }

    switch (ec_val[0])
    {

      case READ_EC:
      printf("case READ_EC\n");
      char ec_tft[5];
      if(strlen(ec_val)==2)
      {
        printf("EC : %.2f\n",0.00);
        sprintf(ec_tft, "%.2f",0.00);
        sprintf(str_name,EC_SEND,ec_tft);
        send_tft(str_name);
      }
      else if(strlen(ec_val)==3)
      {

        float ec_val_plot = ((ec_val[1]-48)*10)+((ec_val[2]-48));
        printf("EC : %.2f\n",ec_val_plot/100);
        sprintf(ec_tft, "%.2f", ec_val_plot/100);
        sprintf(str_name,EC_SEND,ec_tft);
        send_tft(str_name);
      }
      else if(strlen(ec_val)==4)
      {
        float ec_val_plot = ((ec_val[1]-48)*100)+((ec_val[2]-48)*10)+((ec_val[3]-48));
        printf("EC : %.2f\n",ec_val_plot/100);
        sprintf(ec_tft, "%.2f", ec_val_plot/100);
        sprintf(str_name,EC_SEND,ec_tft);
        send_tft(str_name);
      }
      break;
    }
    vTaskDelay(100/ portTICK_PERIOD_MS);
  }
}

void RECV_CALL_PH(void *pvParameter)
{

  while(1)
  {
    adc2_get_raw(PH_CH, ADC_WIDTH_BIT_12, &read_raw);
    adc_reading_ph = read_raw;
    // return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    // printf("PH adc: %.2f\n", adc_reading_ph);
    printf("ADC: %d\n", adc_reading_ph/10);
    // float test = adc_reading_ph*3.3/3959;
    // printf("Volt: %.1f\n", test);
    /////////////// ph read DF Lbr//////////////
    float ph_cb = readPH(adc_reading_ph/10);
    ph_cb = ph_cb-1.0;
    //////////////// calibration ////////////////
    char ph_tft[5];
    printf("PH Val : %.1f\n",ph_cb);
    sprintf(ph_tft, "%.1f", ph_cb);
    sprintf(str_name,PH_SEND,ph_tft);
    send_tft(str_name);
    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
}



void app_main()
{

  nvs_flash_init();
  scani2c(); /* Scan Address I2C */
  init_tft();  /* init lcd nextion */

  vTaskDelay(100/ portTICK_PERIOD_MS);
  TFT_RESTART(); /* TFT Restart */
  vTaskDelay(100/ portTICK_PERIOD_MS);

  /*init chiller */
  gpio_pad_select_gpio(RELAY_GPIO);
  gpio_set_direction(RELAY_GPIO, GPIO_MODE_OUTPUT);
  gpio_set_level(RELAY_GPIO, 0);
  init_faninside();
  init_fanintake();
  waterlevel_init1(); /*init waterlv1 */
  waterlevel_init2(); /*init waterlv2 */
  init_ec(); /* init ec sensor */
  initpca9685(); /* init pca9685 16 ch */
  app_buzzer_cfg(); /* init buzzer */
  init_ph(); /* init ph sensor */

  vTaskDelay(100/ portTICK_PERIOD_MS);
  xTaskCreate(RECV_CALL_TFT, "RECV_CALL_TFT", 2048, NULL, 5, NULL);
  vTaskDelay(200/ portTICK_PERIOD_MS);
  xTaskCreate(&DHT_task, "DHT_task", 2048, NULL, 1, NULL );
  vTaskDelay(200/ portTICK_PERIOD_MS);
  xTaskCreate(&RECV_CALL_EC, "RECV_CALL_EC", 2048, NULL, 1, NULL );
  vTaskDelay(200/ portTICK_PERIOD_MS);
  xTaskCreate(&RECV_CALL_PH, "RECV_CALL_PH", 2048, NULL, 1, NULL );
  xTaskCreate(&waterlv, "waterlv", 2048, NULL, 1, NULL );
  
  // xTaskCreate(&RECV_CALL_PH, "RECV_CALL_PH", 2048, NULL, 1, NULL );

}

#ifndef __MAIN_H__
#define __MAIN_H__


//fade function
char dtmp[64];
uint8_t val_ratio1;
uint8_t val_ratio2;
uint8_t val_ratio3;

////////////////gpio pca9685/////////////

#define ADDR PCA9685_ADDR_BASE
#define SDA_GPIO 21
#define SCL_GPIO 22
#define LED_CH1 0
#define LED_CH2 1
#define LED_CH3 2
#define LED_CH4 3

#define CHECK_LOOP(X, msg, ...) do { esp_err_t __; while((__ = X) != ESP_OK) { printf(msg "\n", ## __VA_ARGS__); vTaskDelay(250 / portTICK_PERIOD_MS); }} while (0)
///////////////////////////////////////
//LCD
char str_name[32];
#define TFTEND                "\xFF\xFF\xFF"
#define TFT_REST              "rest" TFTEND
#define TEMP_SEND             "page0.t0.txt=\"%s C\"" TFTEND
#define TEMP_SEND2            "page0.t4.txt=\"%s C\"" TFTEND
#define TEMP_SEND3            "page0.t5.txt=\"%s C\"" TFTEND
#define EC_SEND               "page0.t2.txt=\"%s ms/cm\"" TFTEND
#define SET_RATIO_1           "page0.h0.val=%d" TFTEND
#define SET_RATIO_2           "page0.h1.val=%d" TFTEND
#define SET_RATIO_3           "page0.h2.val=%d" TFTEND
#define SET_BRIGHTNESS        "page0.h3.val=%d" TFTEND
#define PH_SEND             "page0.t3.txt=\"%s\"" TFTEND

// #define SWITCH_ON_OFF         "sw%03db.val=%d" TFTEND
// #define STATE_ONLINE          "list_01b.st%03d.pic=%d" TFTEND   //28 off, 27 on
// #define SET_R                 "set_device.r_val.val=%d" TFTEND

enum tft_cmd
{

        PAGE_RATIO      =       0x20,
        RATIO           =       0x2A,
        BRIGHTNESS      =       0x2B,
        ERASE_FLASH     =       0x10,
        CALIBRAT_EC     =       0x11,
        CALIBRAT_PH     =       0x12,


        READ_EC        =        0x41,
};

//gpio dht22
#define dht22_gpio 33
#define dht22_gpio2 23
#define dht22_gpio3 32

//gpio chiller
#define RELAY_GPIO 14


#endif

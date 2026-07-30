#ifndef _KEY_H
#define _KEY_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"   
#include "driver/gpio.h"


#define  KEY_L1_IO_NUM     0
#define  KEY_L2_IO_NUM     8

#define  KEY_R1_IO_NUM     17
#define  KEY_R2_IO_NUM     18
#define  KEY_R3_IO_NUM     16

#define  KEY_C1_IO_NUM     9
#define  KEY_C2_IO_NUM     14
#define  KEY_C3_IO_NUM     21
#define  KEY_C4_IO_NUM     47
#define  KEY_C5_IO_NUM     45

#define  KEY_POWER_IO_NUM  42
#define  KEY_FLY_IO_NUM    15

#define KEY_L1 !gpio_get_level(KEY_L1_IO_NUM)
#define KEY_L2 !gpio_get_level(KEY_L2_IO_NUM)
#define KEY_R1 !gpio_get_level(KEY_R1_IO_NUM)
#define KEY_R2 !gpio_get_level(KEY_R2_IO_NUM)
#define KEY_R3 !gpio_get_level(KEY_R3_IO_NUM)
#define KEY_C1 !gpio_get_level(KEY_C1_IO_NUM)
#define KEY_C2 !gpio_get_level(KEY_C2_IO_NUM)
#define KEY_C3 !gpio_get_level(KEY_C3_IO_NUM)
#define KEY_C4 !gpio_get_level(KEY_C4_IO_NUM)
#define KEY_C5 !gpio_get_level(KEY_C5_IO_NUM)
#define KEY_POWER !gpio_get_level(KEY_POWER_IO_NUM)
#define KEY_FLY !gpio_get_level(KEY_FLY_IO_NUM)



void key_init(void);
uint8_t key_scan(void);


#endif
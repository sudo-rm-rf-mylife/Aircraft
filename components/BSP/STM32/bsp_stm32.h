#ifndef _BSP_STM32_H
#define _BSP_STM32_H

#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"

#define STM32_RST_IO_NUM 40

#define  STM32_RST_LOW()  gpio_set_level(STM32_RST_IO_NUM,0)
#define  STM32_RST_HIGH() gpio_set_level(STM32_RST_IO_NUM,1)


void bsp_stm32_init(void);

#endif
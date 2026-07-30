#ifndef  _BSP_LED_H_
#define  _BSP_LED_H_

#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"

#define LED0_IO_NUM  40
#define LED1_IO_NUM  39

#define BEEP_IO_NUM 38

#define LED0_ON()  gpio_set_level(LED0_IO_NUM,0)
#define LED0_OFF() gpio_set_level(LED0_IO_NUM,1)
#define LED0_TOGGLE() gpio_set_level(LED0_IO_NUM,!gpio_get_level(LED0_IO_NUM))

#define LED1_ON()  gpio_set_level(LED1_IO_NUM,0)
#define LED1_OFF() gpio_set_level(LED1_IO_NUM,1)
#define LED1_TOGGLE() gpio_set_level(LED1_IO_NUM,!gpio_get_level(LED1_IO_NUM))


#define BEEP_ON()  gpio_set_level(BEEP_IO_NUM,1)
#define BEEP_OFF() gpio_set_level(BEEP_IO_NUM,0)
#define BEEP_TOGGLE() gpio_set_level(BEEP_IO_NUM,!gpio_get_level(BEEP_IO_NUM))

#define POWER_ON() gpio_set_level(PIN_NUM_BCKL, 1)
#define POWER_OFF() gpio_set_level(PIN_NUM_BCKL, 0)
#define POWER_TOGGLE() gpio_set_level(PIN_NUM_BCKL,!gpio_get_level(PIN_NUM_BCKL))

void Bsp_LED_Init(void);
void Bsp_BEEP_Init(void);



#endif
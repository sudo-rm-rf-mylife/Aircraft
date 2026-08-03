#include "bsp_stm32.h"
#include "driver/gpio.h"
#include "hal/gpio_types.h"
void bsp_stm32_init(void)
{
    gpio_config_t io_conf={};
    io_conf.pin_bit_mask=(1ULL<<STM32_RST_IO_NUM);
    /*不使用中断*/
    io_conf.intr_type=GPIO_INTR_DISABLE;
    io_conf.mode=GPIO_MODE_INPUT_OUTPUT;
    io_conf.pull_down_en=0;
    io_conf.pull_up_en=0;
    gpio_config(&io_conf);
    STM32_RST_HIGH();
}



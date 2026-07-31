#include "bsp_led.h"

void Bsp_Power_Init(void)
{
    gpio_config_t io_conf = {
        .pin_bit_mask = 1ULL << POWER_EN_IO_NUM,
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };

    gpio_config(&io_conf);
    POWER_HOLD();
}

void Bsp_LED_Init(void)
{
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << LED0_IO_NUM) | (1ULL << LED1_IO_NUM),
        .mode = GPIO_MODE_INPUT_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };

    gpio_config(&io_conf);
    LED0_OFF();
    LED1_OFF();
}

void Bsp_BEEP_Init(void)
{
    gpio_config_t io_conf = {
        .pin_bit_mask = 1ULL << BEEP_IO_NUM,
        .mode = GPIO_MODE_INPUT_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };

    gpio_config(&io_conf);
    BEEP_OFF();
}

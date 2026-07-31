#include "key.h"

static const gpio_num_t key_pins[] = {
    KEY_L1_IO_NUM,
    KEY_L2_IO_NUM,
    KEY_R1_IO_NUM,
    KEY_POWER_IO_NUM,
    KEY_R2_IO_NUM,
    KEY_R3_IO_NUM,
    KEY_C1_IO_NUM,
    KEY_C2_IO_NUM,
    KEY_C3_IO_NUM,
    KEY_C4_IO_NUM,
    KEY_C5_IO_NUM,
    KEY_FLY_IO_NUM,
};
void key_init(void)
{
    gpio_config_t io_conf1 = {
        .pin_bit_mask = (1ULL << KEY_L1_IO_NUM) |
                        (1ULL << KEY_L2_IO_NUM) |
                        (1ULL << KEY_R1_IO_NUM) |
                        (1ULL << KEY_R2_IO_NUM) |
                        (1ULL << KEY_R3_IO_NUM) |
                        (1ULL << KEY_C1_IO_NUM) |
                        (1ULL << KEY_C2_IO_NUM) |
                        (1ULL << KEY_C3_IO_NUM) |
                        (1ULL << KEY_C4_IO_NUM) |
                        (1ULL << KEY_C5_IO_NUM) |
                        (1ULL << KEY_FLY_IO_NUM),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    gpio_config(&io_conf1);
    gpio_config_t io_conf2={.pin_bit_mask=(1ULL<<KEY_POWER_IO_NUM),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    gpio_config(&io_conf2);
}

static uint8_t read_pressed_key(void)
{
    uint8_t key_value = 0;

    for (uint8_t i = 0; i < (uint8_t)(sizeof(key_pins) / sizeof(key_pins[0])); ++i)
    {
        if (gpio_get_level(key_pins[i]) == 0)
        {
            key_value = i + 1;
        }
    }

    return key_value;
}
uint8_t key_scan(void)
{
    static bool was_released = true;/*????????*/
    uint8_t key_value = read_pressed_key();/*?????*/

    if (was_released && key_value != 0)/*?????????????*/
    {
        vTaskDelay(pdMS_TO_TICKS(20));
        key_value = read_pressed_key();
        if (key_value != 0)
        {
            was_released = false;
            return key_value;
        }
    }
    was_released = (key_value == 0);/*?????????????*/
    return 0;
}

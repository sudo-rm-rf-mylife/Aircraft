#ifndef BSP_ADC_H
#define BSP_ADC_H

#include <stdint.h>

#include "esp_adc/adc_oneshot.h"

void bsp_adc1_init(void);
uint32_t adc_get_result_average(uint32_t channel, uint32_t sample_count);

#endif

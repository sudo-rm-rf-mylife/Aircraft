#ifndef BSP_ADC_H
#define BSP_ADC_H

#include <stdint.h>

#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali_scheme.h"
#include "driver/adc_types_legacy.h"
#include "esp_adc/adc_cali.h"
#include "esp_err.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "hal/adc_types.h"
#include <stdlib.h>


void bsp_adc1_init(void);
uint32_t adc_get_result_average(uint32_t channel, uint32_t sample_count);
void adc_calibration_init(void);
int dc_raw_to_voltage(adc_cali_handle_t chnnelx_cali_handle,int raw);
int joystick_value(int voltage_mv,int center_mv);
int battery_percent_from_mv(int mv);



#endif

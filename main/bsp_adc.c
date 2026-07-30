#include "bsp_adc.h"

#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static adc_oneshot_unit_handle_t adc1_handle;
void bsp_adc1_init(void)
{
    /*防止重复初始化*/
    if (adc1_handle != NULL)
    {
        return;
    }
    /*配置ADC单元*/
    const adc_oneshot_unit_init_cfg_t unit_config = {
        .unit_id = ADC_UNIT_1,
        .ulp_mode = ADC_ULP_MODE_DISABLE,
    };
     /*配置通道通用参数*/
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&unit_config, &adc1_handle));
    const adc_oneshot_chan_cfg_t channel_config = {
        .atten = ADC_ATTEN_DB_12,
        .bitwidth = ADC_BITWIDTH_12,
    };
    static const adc_channel_t channels[] = {
        ADC_CHANNEL_1,
        ADC_CHANNEL_3,
        ADC_CHANNEL_4,
        ADC_CHANNEL_5,
        ADC_CHANNEL_6,
    };
    for (uint32_t i = 0; i < (uint32_t)(sizeof(channels) / sizeof(channels[0])); ++i)
    {
        ESP_ERROR_CHECK(adc_oneshot_config_channel(adc1_handle, channels[i], &channel_config));
    }
}
/*获取ADC采样平均值（测试用）*/
uint32_t adc_get_result_average(uint32_t channel, uint32_t sample_count)
{
    uint64_t sum = 0;
    uint32_t successful_samples = 0;

    for (uint32_t i = 0; i < sample_count; ++i)
    {
        int raw_value = 0;
        if (adc_oneshot_read(adc1_handle, (adc_channel_t)channel, &raw_value) == ESP_OK)
        {
            sum += (uint32_t)raw_value;
            ++successful_samples;
        }
        vTaskDelay(pdMS_TO_TICKS(5));
    }

    return successful_samples == 0 ? 0 : (uint32_t)(sum / successful_samples);
}

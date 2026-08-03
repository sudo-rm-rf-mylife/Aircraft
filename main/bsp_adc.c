#include "bsp_adc.h"
#include "esp_adc/adc_cali_scheme.h"
#include "esp_err.h"


#define JOYSTICK_DEAD_ZONE_MV 80


adc_oneshot_unit_handle_t adc1_handle;

adc_cali_handle_t Power_cali_handle = NULL;
adc_cali_handle_t chnnel3_cali_handle = NULL;
adc_cali_handle_t chnnel4_cali_handle = NULL;
adc_cali_handle_t chnnel5_cali_handle = NULL;
adc_cali_handle_t chnnel6_cali_handle = NULL;

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
//初始化校准
void adc_calibration_init(void)
{
      
    adc_cali_curve_fitting_config_t  Power_cali_config={
        .unit_id=ADC_UNIT_1,
        .chan=ADC1_CHANNEL_1,
        .atten=ADC_ATTEN_DB_12,
        .bitwidth=ADC_BITWIDTH_DEFAULT,
        };
    adc_cali_curve_fitting_config_t cali1_config={
        .unit_id=ADC_UNIT_1,
        .chan=ADC1_CHANNEL_3,
        .atten=ADC_ATTEN_DB_12,
        .bitwidth=ADC_BITWIDTH_DEFAULT,
        };
        adc_cali_curve_fitting_config_t cali2_config={
        .unit_id=ADC_UNIT_1,
        .chan=ADC1_CHANNEL_4,
        .atten=ADC_ATTEN_DB_12,
        .bitwidth=ADC_BITWIDTH_DEFAULT,
        };
        adc_cali_curve_fitting_config_t cali3_config={
        .unit_id=ADC_UNIT_1,
        .chan=ADC1_CHANNEL_5,
        .atten=ADC_ATTEN_DB_12,
        .bitwidth=ADC_BITWIDTH_DEFAULT,
        };
        adc_cali_curve_fitting_config_t cali4_config={
        .unit_id=ADC_UNIT_1,
        .chan=ADC1_CHANNEL_6,
        .atten=ADC_ATTEN_DB_12,
        .bitwidth=ADC_BITWIDTH_DEFAULT,
        };
        
        //为 ADC（模数转换器）创建一个“曲线拟合”校准方案
        esp_err_t power_ret=adc_cali_create_scheme_curve_fitting(
        &Power_cali_config,&Power_cali_handle);
        esp_err_t ret1=adc_cali_create_scheme_curve_fitting(
        &cali1_config, &chnnel3_cali_handle);
        esp_err_t ret2=adc_cali_create_scheme_curve_fitting(
        &cali2_config, &chnnel4_cali_handle);
        esp_err_t ret3=adc_cali_create_scheme_curve_fitting(
        &cali3_config, &chnnel5_cali_handle);
        esp_err_t ret4=adc_cali_create_scheme_curve_fitting(
        &cali4_config, &chnnel6_cali_handle);
        if (power_ret == ESP_OK)
        {
        printf("ADC calibration ready\n");
        } 
        else 
        {
        printf("ADC calibration failed: %s\n",
        esp_err_to_name(power_ret));
        }
        if(ret1==ESP_OK)
        {
        printf("chnnel3 calibration enabled\n");
        }
        else 
        {
        printf("chnnel3 calibration unavailable\n");
        chnnel3_cali_handle = NULL;
        }
        if(ret2==ESP_OK)
        {
        printf("chnnel4 calibration enabled\n");
        }
        else 
        {
        printf("chnnel4 calibration unavailable\n");
        chnnel4_cali_handle = NULL;
        }
        if(ret3==ESP_OK)
        {
        printf("chnnel5 calibration enabled\n");
        }
        else 
        {
        printf("chnnel5 calibration unavailable\n");
        chnnel5_cali_handle = NULL;
        }
        if(ret4==ESP_OK)
        {
        printf("chnnel6 calibration enabled\n");
        }
        else 
        {
        printf("chnnel6 calibration unavailable\n");
        chnnel6_cali_handle = NULL;
        }
}
int dc_raw_to_voltage(adc_cali_handle_t chnnelx_cali_handle,int raw)
{
    int voltage_mv=0;
    if(chnnelx_cali_handle!=NULL)
    {
        adc_cali_raw_to_voltage(chnnelx_cali_handle ,raw,&voltage_mv);
    }
    else 
    {
    //无校准时使用近似公式
    voltage_mv =raw*3300/4095;
    }
    return voltage_mv;
}
int joystick_value(int voltage_mv,int center_mv)
{
    int difference;
    int value;
    difference=voltage_mv-center_mv;
    //死区以内认为遥杆没有动作
    if(abs(difference)<=JOYSTICK_DEAD_ZONE_MV)
    {
        return 0;
    }
    //扣除死区，避免刚越过死区时数值突然跳变
    if(difference>0)
    {
        difference-=JOYSTICK_DEAD_ZONE_MV;
    }
    else {
    difference+=JOYSTICK_DEAD_ZONE_MV;
    }
    if(difference>0)
    {
        value=difference*100/(3300-center_mv-JOYSTICK_DEAD_ZONE_MV);
    }
    else 
    {
        value=difference*100/(center_mv-JOYSTICK_DEAD_ZONE_MV);
    }
    if (value > 100) {
        value = 100;
    }
    if (value < -100) {
        value = -100;
    }
    return value;
}
int battery_percent_from_mv(int mv)
{
    int ret=mv*2;
    if (ret>= 4200) return 100;
    else if (ret >= 4000) return 90;
    else if (ret >= 3980) return 80;
    else if (ret >= 3920) return 70;
    else if (ret >= 3870) return 60;
    else if (ret>= 3820) return 50;
    else if (ret >= 3790) return 40;
    else if (ret >= 3740) return 20;
    else if (ret>= 3680) return 10;
    else if (ret>= 3450) return 5;
    else 
    {
        return 0;
    }
}
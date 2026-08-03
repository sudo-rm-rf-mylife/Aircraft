#include"main.h"
#include "driver/adc_types_legacy.h"
#include "smartconfig.h"

#define CENTER_ROLL         1655
#define CENTER_PITCH        1619
#define CENTER_THRUST       1669
#define CENTER_YAW          1659

TaskHandle_t key_scan_task_handle;
TaskHandle_t data_task_handle;
TaskHandle_t Power_task_handle;
TaskHandle_t Rocker_task_handle;

QueueHandle_t xQueueLED0;
QueueHandle_t xQueueLED1;
QueueHandle_t xQueueBEEP;
QueueHandle_t xQueuePower;
QueueHandle_t xQueueRocker;

int pitch_raw;
int roll_raw;
int thrust_raw;
int yaw_raw;
int power_raw;


int pitch_raw_voltage;
int roll_raw_voltage;
int thrust_raw_voltage;
int yaw_raw_voltage;
int power_raw_voltage;

int pitch_mv;
int roll_mv;
int thrust_mv;
int yaw_mv;
int power_mv;

extern const unsigned char gImage_image01[153600];
extern const char matrix[][16];
extern const unsigned char font_library_8_16[][16];
extern adc_oneshot_unit_handle_t adc1_handle;
extern adc_cali_handle_t Power_cali_handle ;
extern adc_cali_handle_t chnnel3_cali_handle;
extern adc_cali_handle_t chnnel4_cali_handle;
extern adc_cali_handle_t chnnel5_cali_handle;
extern adc_cali_handle_t chnnel6_cali_handle;


void key_scan_task(void *para);
void data_task(void *para); 
void Power_task(void *para);
void vPowerCallback(TimerHandle_t xTimer);
void Rocker_task(void *para);

void app_main(void)
{
    /* Hold the hardware power latch before lengthy peripheral initialization. */
    Bsp_Power_Init();
    Bsp_LCD_Init();
    Bsp_LED_Init();
    key_init();
    Bsp_BEEP_Init();
    bsp_adc1_init();
    adc_calibration_init();
    nvs_init();
    //lcd_show_pic(gImage_image01);
    smartwifi_init();
    //http_request_connect();

    xQueueLED0 = xQueueCreate(10, sizeof(int));
    xQueueLED1 = xQueueCreate(10, sizeof(int));
    xQueueBEEP = xQueueCreate(10, sizeof(int));
    xQueuePower = xQueueCreate(10, sizeof(int));
    

    xTaskCreate(key_scan_task, "key_scan", 4096, NULL, 4, &key_scan_task_handle);
    xTaskCreate(data_task, "data", 4096, NULL, 5, &data_task_handle);
    xTaskCreate(Power_task,"Power",4096,NULL,25,&Power_task_handle); 
    xTaskCreate(Rocker_task,"Rocker",4096,NULL,6,&Rocker_task_handle);    
    while (1)
    {
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
void key_scan_task(void *para)
{
    int val = 0;
    for (;;)
    {
        val = key_scan();
        if (val == 5)
        {
            xQueueSend(xQueueLED0, &val, 0);
        }
        else if (val == 6)
        {
            xQueueSend(xQueueLED1, &val, 0);
        }
        else if (val == 7)
        {
            xQueueSend(xQueueBEEP, &val, 0);
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
void data_task(void *para)
{
    int data_led0;
    int data_led1;
    int data_beep;
    for (;;)
    {
        if (xQueueReceive(xQueueLED0, &data_led0, 0) == pdTRUE)
        {
            LED0_TOGGLE();
        }
        if (xQueueReceive(xQueueLED1, &data_led1, 0) == pdTRUE)
        {
            LED1_TOGGLE();
        }
        if (xQueueReceive(xQueueBEEP, &data_beep, 0) == pdTRUE)
        {
            BEEP_TOGGLE();
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
void Power_task(void *para)
{
    const TickType_t power_off_hold_ticks = pdMS_TO_TICKS(1000);

    /* Do not treat the button press used to power on as a shutdown request. */
    while (KEY_POWER)
    {
        vTaskDelay(pdMS_TO_TICKS(10));
    }
    vTaskDelay(pdMS_TO_TICKS(20));

    for (;;)
    {
        if (KEY_POWER)
        {
            vTaskDelay(pdMS_TO_TICKS(20));
            if (!KEY_POWER)
            {
                continue;
            }

            TickType_t press_start = xTaskGetTickCount();
            while (KEY_POWER)
            {
                if ((xTaskGetTickCount() - press_start) >= power_off_hold_ticks)
                {
                    /* Q1 remains on while the key is held and turns off on release. */
                    POWER_RELEASE();
                    while (KEY_POWER)
                    {
                        vTaskDelay(pdMS_TO_TICKS(10));
                    }
                    vTaskSuspend(NULL);
                }
                vTaskDelay(pdMS_TO_TICKS(10));
            }
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
void Rocker_task(void *para)
{

    char buf[40]={0};
    const char *dir=NULL;
    for(;;)
    {   ESP_ERROR_CHECK(adc_oneshot_read(adc1_handle,ADC1_CHANNEL_1,&power_raw));
        ESP_ERROR_CHECK(adc_oneshot_read(adc1_handle,ADC1_CHANNEL_3,&pitch_raw));
        ESP_ERROR_CHECK(adc_oneshot_read(adc1_handle,ADC1_CHANNEL_4,&roll_raw));
        ESP_ERROR_CHECK(adc_oneshot_read(adc1_handle,ADC1_CHANNEL_5,&thrust_raw));
        ESP_ERROR_CHECK(adc_oneshot_read(adc1_handle,ADC1_CHANNEL_6,&yaw_raw));

        power_raw_voltage=dc_raw_to_voltage(Power_cali_handle,power_raw);
        pitch_raw_voltage=dc_raw_to_voltage(chnnel3_cali_handle,pitch_raw);
        roll_raw_voltage=dc_raw_to_voltage(chnnel4_cali_handle,roll_raw);
        thrust_raw_voltage=dc_raw_to_voltage(chnnel5_cali_handle,thrust_raw);
        yaw_raw_voltage=dc_raw_to_voltage(chnnel6_cali_handle, yaw_raw);
        
        power_mv=battery_percent_from_mv(power_raw_voltage);
        pitch_mv=joystick_value(pitch_raw_voltage,CENTER_PITCH );
        roll_mv=joystick_value(roll_raw_voltage,CENTER_ROLL);
        thrust_mv=joystick_value(thrust_raw_voltage,CENTER_THRUST);
        yaw_mv=joystick_value(yaw_raw_voltage,CENTER_YAW);

        //lcd_fill_rect(0, 0, 319, 63, 0x0000);

        //????
        snprintf(buf, sizeof(buf), "POWER:%d", power_mv);
        lcd_show_str( 256,0, buf, 0x7E0); 

        dir = pitch_mv>0 ? "FORWARD" : pitch_mv<0 ? "BACKWARD" : "CENTER";
        snprintf(buf, sizeof(buf), "PITCH:%d,%s", pitch_mv, dir);
        //printf("%s\n", buf);
        lcd_show_str(0, 0, buf, 0x7E0); 


        dir = roll_mv>0 ? "RIGHT" : roll_mv<0 ? "LEFT" : "CENTER";
        snprintf(buf, sizeof(buf), "ROLL:%d,%s", roll_mv, dir);
        //printf("%s\n", buf);
        lcd_show_str(0, 16, buf, 0x7E0);

        dir = thrust_mv>0 ? "UP" : thrust_mv<0 ? "DOWN" : "CENTER";
        snprintf(buf, sizeof(buf), "THRUST:%d,%s", thrust_mv, dir);
        //printf("%s\n", buf);
        lcd_show_str(0, 32, buf, 0x7E0);

        dir = yaw_mv>0 ? "LEFT" : yaw_mv<0 ? "RIGHT" : "CENTER";
        snprintf(buf, sizeof(buf), "YAW:%d,%s", yaw_mv, dir);
        //printf("%s\n", buf);
        lcd_show_str(0, 48, buf, 0x7E0);
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

#include"main.h"


TaskHandle_t key_scan_task_handle;
TaskHandle_t data_task_handle;
TaskHandle_t Power_task_handle;

QueueHandle_t xQueueLED0;
QueueHandle_t xQueueLED1;
QueueHandle_t xQueueBEEP;
QueueHandle_t xQueuePower;

extern const unsigned char gImage_image01[153600];
extern const char matrix[][16];

void key_scan_task(void *para);
void data_task(void *para); 
void Power_task(void *para);
void vPowerCallback(TimerHandle_t xTimer);

void app_main(void)
{
    /* Hold the hardware power latch before lengthy peripheral initialization. */
    Bsp_Power_Init();
    Bsp_LCD_Init();
    Bsp_LED_Init();
    key_init();
    Bsp_BEEP_Init();
    bsp_adc1_init();

    //lcd_show_pic(gImage_image01);

    xQueueLED0 = xQueueCreate(10, sizeof(int));
    xQueueLED1 = xQueueCreate(10, sizeof(int));
    xQueueBEEP = xQueueCreate(10, sizeof(int));
    xQueuePower = xQueueCreate(10, sizeof(int));

    xTaskCreate(key_scan_task, "key_scan", 4096, NULL, 4, &key_scan_task_handle);
    xTaskCreate(data_task, "data", 4096, NULL, 5, &data_task_handle);
    xTaskCreate(Power_task,"Power",4096,NULL,25,&Power_task_handle); 
    esp_err_t ret = nvs_flash_init();//NV≥ı ºªØ
    if(ret==ESP_ERR_NVS_NO_FREE_PAGES||ret==ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret=nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    for (int i = 0; i <12; i++) 
    {
    lcd_draw_font(10 + i * 8, 20,(const unsigned char *)matrix[i]);
    }
    //wifi_scan();
    //wifi_station_init();
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

#include"main.h"
#include "freertos/projdefs.h"
#include "key.h"
#include <stdint.h>

TaskHandle_t key_scan_task_handle;
TaskHandle_t data_task_handle;
TaskHandle_t Power_task_handle;

QueueHandle_t xQueueLED0;
QueueHandle_t xQueueLED1;
QueueHandle_t xQueueBEEP;
QueueHandle_t xQueuePower;

void key_scan_task(void *para);
void data_task(void *para);
void Power_task(void *para);
void vPowerCallback(TimerHandle_t xTimer);

void app_main(void)
{ 
    Bsp_LCD_Init();
    key_init();
    Bsp_BEEP_Init();
    bsp_adc1_init();
    
    xQueueLED0 = xQueueCreate(10, sizeof(int));
    xQueueLED1 = xQueueCreate(10, sizeof(int));
    xQueueBEEP = xQueueCreate(10, sizeof(int));
    xQueuePower = xQueueCreate(10, sizeof(int));

    xTaskCreate(key_scan_task, "key_scan", 4096, NULL, 4, &key_scan_task_handle);
    xTaskCreate(data_task, "data", 4096, NULL, 5, &data_task_handle);
    xTaskCreate(Power_task,"Power",4096,NULL,25,&Power_task_handle); 
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
    for(;;)
    {
    if(KEY_POWER) 
    {
    vTaskDelay(pdMS_TO_TICKS(500));
    if(KEY_POWER)
    {
    POWER_TOGGLE();
    }
    }
    vTaskDelay(pdMS_TO_TICKS(5));
    }
}

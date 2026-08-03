#include  "main.h"
#include "bsp_uart.h"


void key_scan_task(void *para);

void app_main(void)
{
    esp_err_t ret;
    size_t len = 0;
    uint16_t times = 0;
    unsigned char data[RX_BUF_SIZE] = {0};
    ret = nvs_flash_init();                                   /* 初始化NVS */
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES ||
        ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
                                                  /* 初始化LED */
    usart_init(115200);                                       /* 初始化串口 */
    while(1)
{
    /* 获取环形缓冲区数据长度 */
    uart_get_buffered_data_len(STM32_UART_UX, &len);
    if (len > 0)                                          /* 判断数据长度 */
    {
        size_t read_len = len < (sizeof(data) - 1) ? len : (sizeof(data) - 1);
        int received = uart_read_bytes(STM32_UART_UX, data, read_len, pdMS_TO_TICKS(100));
        if (received > 0) {
        printf("Received %d bytes from UART2\n", received);
        }
    }
    else
    {
    times++;
    if (times % 5000 == 0)
    {
                printf("\nATK-DNESP32-S3 UART test\n");
                printf("ALIENTEK\n\n\n");
    }
    if (times % 200 == 0)
    {
                printf("Send data and press Enter\n");
    }
    if (times % 30 == 0)
    {
        LED1_TOGGLE();
    }
    vTaskDelay(10);
        }
    }
}

#include "bsp_uart.h"
/**
*@brief       初始化串口
*@param       baudrate: 波特率, 根据自己需要设置波特率值
*@note        注意: 必须设置正确的时钟源, 否则串口波特率就会设置异常.
*@retval      无
*/
void usart_init(uint32_t baudrate)
{
   uart_config_t uart_config;                             /* 串口配置句柄 */
   uart_config.baud_rate = baudrate;                     /* 波特率 */
   uart_config.data_bits =UART_DATA_8_BITS;            /* 字长为8位数据格式 */
   uart_config.parity =UART_PARITY_DISABLE;            /* 无奇偶校验位 */
   uart_config.stop_bits =UART_STOP_BITS_1;            /* 一个停止位 */
   uart_config.flow_ctrl =UART_HW_FLOWCTRL_DISABLE;      /* 无硬件控制流 */
   uart_config.source_clk = UART_SCLK_APB;              /* 配置时钟源 */
   uart_config.rx_flow_ctrl_thresh = 122;               /* 硬件控制流阈值 */
   uart_param_config(STM32_UART_UX , &uart_config);           /* 配置uart端口 */
    /* 配置uart引脚 */
    uart_set_pin(STM32_UART_UX ,
    STM32_USART_TX_GPIO_PIN,
                  STM32_USART_RX_GPIO_PIN,
                 UART_PIN_NO_CHANGE,
                 UART_PIN_NO_CHANGE);
    /* 安装串口驱动 */
    uart_driver_install(STM32_UART_UX ,
                        RX_BUF_SIZE* 2,
                        RX_BUF_SIZE* 2,
                        20,
                        NULL,
                        0);
}
void stm32_uart_printf(const char *format, ...)
{
   char buffer[UART_PRINTF_BUFFER_SIZE];
   va_list args;
    
   // 格式化字符串
   va_start(args, format);
   vsnprintf(buffer, sizeof(buffer), format, args);
   va_end(args);
    
   // 通过ESP32串口（连接STM32）发送
   uart_write_bytes(STM32_UART_UX, (uint8_t*)buffer, strlen(buffer));    /* 写数据 */
}
#ifndef _BSP_UART_H
#define _BSP_UART_H

#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"
#include "driver/uart.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

/* 引脚和串口定义 */
#define STM32_UART_UX                 UART_NUM_2
#define STM32_USART_TX_GPIO_PIN       GPIO_NUM_2
#define STM32_USART_RX_GPIO_PIN       GPIO_NUM_45

#define UART_PRINTF_BUFFER_SIZE     1024

/* 串口接收相关定义 */
#define RX_BUF_SIZE              1024    /* 环形缓冲区大小 */

void usart_init(uint32_t baudrate);


#endif
#ifndef BSP_LCD_H
#define BSP_LCD_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"
//#include "pretty_effect.h"

#define CONFIG_LCD_TYPE_AUTO 
#define LCD_HOST    SPI2_HOST

#define PIN_NUM_MISO    -1
#define PIN_NUM_MOSI    13
#define PIN_NUM_CLK     12
#define PIN_NUM_CS      46

#define PIN_NUM_DC      10
#define PIN_NUM_RST     41
#define PIN_NUM_BCKL    3
#define PARALLEL_LINES 16


typedef struct {
    uint8_t cmd;
    uint8_t data[16];
    uint8_t databytes; //No of data in data; bit 7 = delay after set; 0xFF = end of cmds.
} lcd_init_cmd_t;

typedef enum {
    LCD_TYPE_ILI = 1,
    LCD_TYPE_ST,
    LCD_TYPE_MAX,
} type_lcd_t;

void Bsp_LCD_Init(void);
void lcd_clear(uint16_t color);
void lcd_show_pic(const uint8_t *pcolor);
//void lcd_draw_font(uint16_t x, uint16_t y, const unsigned char *font);

#endif

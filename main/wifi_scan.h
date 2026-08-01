#ifndef _WIFI_SCAN_H
#define _WIFI_SCAN_H

#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "esp_event.h"
#include "nvs_flash.h"


#define DEFAULT_SCAN_LIST_SIZE 10

void wifi_scan(void);
void nvs_init(void);

#endif
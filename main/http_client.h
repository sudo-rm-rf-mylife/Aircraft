#ifndef _HTTP_CLIENT_H
#define _HTTP_CLIENT_H

#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"
#include "lwip/dns.h"
#include "sdkconfig.h"
#include "esp_http_client.h"
#include "esp_log.h"

/* Constants that aren't configurable in menuconfig */
#define WEB_SERVER "www.baidu.com"
#define WEB_PORT "80"
#define WEB_PATH "/"

void http_request_connect(void);


#endif

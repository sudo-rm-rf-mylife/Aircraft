#include "http_client.h"
#include "esp_err.h"
#include "esp_http_client.h"
#include "esp_log.h"
#include "freertos/portmacro.h"

static const char *TAG="HTTP_CLIENT";
//这个回调函数用来处理接收到的数据
esp_err_t _http_event_handler(esp_http_client_event_t *evt)
{
    switch (evt->event_id)  
    {
        case HTTP_EVENT_ON_DATA:
        //当接收到数据包时触发，直接将数据打印出来
        if(evt->data_len>0)
        {
            //此处用fwite确保完整输出，也可以用printf或ESP_LOGI
            fwrite(evt->data,1,evt->data_len,stdout);
        }
            break;

        case HTTP_EVENT_ERROR:
        ESP_LOGE(TAG, "HTTP error occurred");
        return ESP_FAIL;

        default:
            break;
    }
    return ESP_OK;
}
static void http_get_task(void *pvParameters)
{
    while(1)
    {
        //1.配置客户端参数
        esp_http_client_config_t config={
            .url="http://"WEB_SERVER":"WEB_PORT WEB_PATH,//构建完整URL
            .method=HTTP_METHOD_GET,//发送一个拿数据的请求
            .timeout_ms=5000,
            .event_handler=_http_event_handler,//设置数据接收回调
            .buffer_size=1024,//接收缓冲区大小
        };
        //2.初始化客户端
        esp_http_client_handle_t client=esp_http_client_init(&config);
        if(client==NULL)
        {
            ESP_LOGE(TAG,"Failed to initialize HTTP client");
            vTaskDelay(1000/portTICK_PERIOD_MS);
            continue;     
        }
        //3.执行请求（这个函数会阻塞知道完成或出错）
        esp_err_t err=esp_http_client_perform(client);
        if (err==ESP_OK) {
            //4.请求成功，获取状态码和内容长度
            int status_code=esp_http_client_get_status_code(client);
            int content_length=esp_http_client_get_content_length(client);
            ESP_LOGI(TAG,"HTTP Status=%d,Content Lenth=%d",status_code,content_length);
        }
        else {
        {
            //处理错误
            ESP_LOGE(TAG,"HTTP request failed:%s",esp_err_to_name(err));
        }
        //5.清理资源
        esp_http_client_cleanup(client);
        //6.等待10秒后再次发起请求
        for (int countdown = 10; countdown >= 0; countdown--) {
        ESP_LOGI(TAG, "%d... ", countdown);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        }
        ESP_LOGI(TAG, "Starting again!");
        }
        }
}
void http_request_connect(void)
{
    // 创建任务，分配稍大些的堆栈（ESP_HTTP_CLIENT需要更多栈空间）
    xTaskCreate(http_get_task, "http_get_task", 8192, NULL, 5, NULL);
}

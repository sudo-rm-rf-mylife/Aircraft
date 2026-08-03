idf_component_register(
    SRCS "main.c"
    INCLUDE_DIRS "."
    REQUIRES
        BSP
        RESOURCE
        driver
        esp_adc
        esp_event
        esp_netif
        esp_wifi
        freertos
        nvs_flash
        esp_system
)

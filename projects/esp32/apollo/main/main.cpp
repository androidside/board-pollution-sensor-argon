#include <stdio.h>

#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define TAG "CONFIG"

// see https://www.learnesp32.com/7_menuconfig-introduction for details
void config(void *params)
{
    ESP_LOGI(TAG, "MY_INT %d", CONFIG_MY_INT);
    ESP_LOGI(TAG, "MY_STRING %s", CONFIG_MY_STRING);
  
    bool my_bool = false;
  #ifdef CONFIG_MY_BOOL
    my_bool = true;
  #else
    my_bool = false;
  #endif
  
    ESP_LOGI(TAG, "MY_BOOL %s", my_bool ? "yes" : "no");
  
    int option = 0;
  
  #ifdef CONFIG_OPTION_1
        option = 1;
  #elif CONFIG_OPTION_2
        option = 2;
  #else
        option = 3;
  #endif
  
   ESP_LOGI(TAG, "MY_OPTION %d", option);
   while(true)
      vTaskDelay(1000 / portTICK_PERIOD_MS);
}
extern "C" void app_main(void)
{
  printf("Hello world!\n");
  
  xTaskCreate(&config, "config demo", 2048, NULL, 2, NULL);
}

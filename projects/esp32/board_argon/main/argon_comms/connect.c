  
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_event_loop.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "cJSON.h"
#include "fetch.h"

#define SSID CONFIG_WIFI_SSID
#define PASSWORD CONFIG_WIFI_PASSWORD

char *TAG = "CONNECTION";

extern xSemaphoreHandle onConnectionHandler;
extern int testextern;

//Method used on the esp_event_loop_init() function on the wifiInit function
//This method captures/intercepts the states of the cycles that happen when we connect to the internet
static esp_err_t event_handler(void *ctx, system_event_t *event)
{
  switch (event->event_id)
  {
  case SYSTEM_EVENT_STA_START:
    esp_wifi_connect();
    ESP_LOGI(TAG,"connecting ...\n");
    break;

  case SYSTEM_EVENT_STA_CONNECTED:
    ESP_LOGI(TAG,"connected...\n");
    break;

  case SYSTEM_EVENT_STA_GOT_IP:
    ESP_LOGI(TAG,"got ip...\n");
    //Gives me back the amount of space left on the stack
    printf("stack space is %d\n", uxTaskGetStackHighWaterMark(NULL));
    //We give the seamaphore so the task can continue
    xSemaphoreGive(onConnectionHandler);
    
    break;

  case SYSTEM_EVENT_STA_DISCONNECTED:
    ESP_LOGI(TAG,"disconnected...\n");
    break;

  default:
    break;
  }
  return ESP_OK;
}

void wifiInit()
{
  //Initialize Non volatile storage NVS
  ESP_ERROR_CHECK(nvs_flash_init());
  //Initialize my TCP adapter
  tcpip_adapter_init();

  //Handle an event loop
  ESP_ERROR_CHECK(esp_event_loop_init(event_handler, NULL));

  //Initialize our wifi configuration
  wifi_init_config_t wifi_init_config = WIFI_INIT_CONFIG_DEFAULT();

  //Tell ESP to use this structure that we just declared
  ESP_ERROR_CHECK(esp_wifi_init(&wifi_init_config));

  //Tell the esp whether or not is going to be utilizing our wifi as a station mode softapp (software access point)
  //WIFI_MODE_AP == I can connect other things to it
  //WIFI_MODE_STA == You connect to a router
  ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));

  wifi_config_t wifi_config = {
      .sta = {
          .ssid = SSID,
          .password = PASSWORD}};

  //We set this configuration
  esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config);
  ESP_ERROR_CHECK(esp_wifi_start());
}


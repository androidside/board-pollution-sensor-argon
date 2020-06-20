#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <string.h>
#include "nvs.h"
#include "nvs_flash.h"
#include "freertos/semphr.h"
#include "freertos/event_groups.h"
#include "esp_spiffs.h"
#include <stdlib.h>
#include <dirent.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include "driver/gpio.h"
#include "driver/dac.h"
#include "driver/adc.h"
#include "driver/ledc.h"
#include "driver/touch_pad.h"
#include "esp_sleep.h"
#include "esp_log.h"
#include "esp32/rom/uart.h"
#include "driver/rtc_io.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "esp_http_client.h"
// #include "argon_comms/fetch.h"
#include "cJSON.h"
#include "gps.h"
#include "lm75a.h"
#include "gps.h"
// #include "argon_comms/connect.h"
// #include "argon_reading/reading.h"
// #include "argon_monitoring/memory.h"
// #include "argon_comms/connect.h"
// #include "argon_reading/lm75a.h"
// #include "argon_reading/no2.h"
#include "gpsapi.h"
#include "no2.h"
#include "tmp102.h"
#include "sdcard.h"

#define TAG "TASKS"

void readSensors(void *params)
{
    while (true)
    {
        readlatitude();
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void app_main()
{
    // startGPS();
    // while (true)
    // {
        
    //     //vTaskDelay(10000 / portTICK_PERIOD_MS); /* code */
    //     readGPS();
    // }
    
    //xTaskCreate(&readSensors, "create reading", 1024 * 8, NULL, 1, NULL);
    //xTaskCreate(&activateGPS, "read gps", 1024 * 8, NULL, 5, NULL);

    //xTaskCreate(&activateNO2, "read no2", 1024 *3, NULL, 5, NULL);
    //xTaskCreate(&activateTMP102, "activate TMPS102", 1024 *2, NULL, 5, NULL);
    xTaskCreate(&activateSDCard, "activate SDCard", 1024 *2, NULL, 5, NULL);
    //xTaskCreate(&activateLM75A, "read LM75A", 1024 * 4, NULL, 5, NULL);
    
}
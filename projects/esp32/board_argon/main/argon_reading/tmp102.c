#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2c.h"
#include "esp_types.h"
#include "esp_log.h"
#include "SparkFunTMP102api.h"
#include "freertos/semphr.h"

// #include "SparkfunADS1015api.h"
// #include "gpsapi.h"

#define TAG_TMP102 "TMP102.C"
float temperature;
extern xSemaphoreHandle mutexBus;

float readTemperatureTMP102()
{
    ESP_LOGI(TAG_TMP102, "readTemperatureTMP102(): %f", temperature);
    return temperature;
}

void activateTMP102(void *params)
{
    if (xSemaphoreTake(mutexBus, 10000 / portTICK_PERIOD_MS))
    {
        startTMP102();
        xSemaphoreGive(mutexBus);
    }

    while (true)
    {
        if (xSemaphoreTake(mutexBus, 1000 / portTICK_PERIOD_MS))
        {
            temperature = readTemperatureTMP102api();
            xSemaphoreGive(mutexBus);
            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }
        else
        {
            ESP_LOGE(TAG_TMP102, "tmp102.c: mutexBus timeout");
        }
    }
}

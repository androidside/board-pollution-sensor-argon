#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2c.h"
#include "esp_types.h"
#include "esp_log.h"
#include "SparkFunTMP102api.h"

#define TAG_TMP102 "TMP102.C"
float temperature;

float readTemperatureTMP102()
{
    ESP_LOGI(TAG_TMP102, "readTemperatureTMP102(): %f", temperature);
    return temperature;
}

void activateTMP102(void *params)
{
    startTMP102();

    while (true)
    {
        temperature = readTemperatureTMP102api();
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

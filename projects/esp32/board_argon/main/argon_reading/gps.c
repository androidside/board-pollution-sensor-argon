#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2c.h"
#include "esp_types.h"
#include "gpsapi.h"
#include "esp_log.h"
#include "freertos/semphr.h"

#define TAG_GPS "GPS_API.C"
float latitude_param;
float longitude_param;
char datetime_param[128] = "-1";
extern xSemaphoreHandle mutexBus;

float getlatitude()
{
    ESP_LOGI(TAG_GPS, "readlatitude(): %f", latitude_param);
    return latitude_param;
}

float getlongitude()
{
    ESP_LOGI(TAG_GPS, "readlongitude(): %f", longitude_param);
    return longitude_param;
}

const char *getdatetime()
{
    ESP_LOGI(TAG_GPS, "readdatetime(): %s", datetime_param);
    return datetime_param;
}

// const char *readlongitude()
// {
//     char *ch = new char[longitudestring.length() + 1] /*  len of string + 1 char for \0*/;
//     strcpy(ch, longitudestring.c_str());
//     return ch;
// }

// const char *readdatetime()
// {
//     char *ch = new char[datetimestring.length() + 1] /*  len of string + 1 char for \0*/;
//     strcpy(ch, datetimestring.c_str());
//     return ch;
// }

void activateGPS(void *params)
{
    //I cannot directly pass fetchParams.body because it is a pointer in memory, I would have to do a malloc. In this case I decide to create a buffer

    if (xSemaphoreTake(mutexBus, 10000 / portTICK_PERIOD_MS))
    {
        startGPS();
        xSemaphoreGive(mutexBus);
    }

    while (true)
    {
        //We block the semaphore
        if (xSemaphoreTake(mutexBus, 1000 / portTICK_PERIOD_MS))
        {
            readGPS(&latitude_param, &longitude_param, datetime_param);
            xSemaphoreGive(mutexBus);
            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }
        else
        {
            ESP_LOGE(TAG_GPS, "gps.c: readGPS() timeout");
        }
    }
}
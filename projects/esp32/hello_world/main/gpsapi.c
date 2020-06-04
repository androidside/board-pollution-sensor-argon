#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2c.h"
#include "esp_types.h"
#include "gps.h"
#include "esp_log.h"

#define TAG_GPS "GPS_API.C"

char latitude_param[64];
char longitude_param[64];
char datetime_param[128];

const char *readlatitude()
{
    ESP_LOGI(TAG_GPS, "latitude_param(): %s", latitude_param);
    ESP_LOGI(TAG_GPS, "longitude_param(): %s", longitude_param);
    ESP_LOGI(TAG_GPS, "datetime_param(): %s", datetime_param);
    return latitude_param;
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

    startGPS();
    while (true)
    {
        readGPS(latitude_param, longitude_param, datetime_param);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
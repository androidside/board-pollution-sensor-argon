#include "freertos/FreeRTOS.h"
#include "reading.h"
#include "esp_system.h"
#include "esp_log.h"
#include "freertos/task.h"


#define TAG_READING "READING"

void populateRandomReading(struct reading_t *reading)
{   
    ESP_LOGI(TAG_READING, "populating Random Reading");
    reading->sensor_id = esp_random()%11 +1; //Random (0,10) + 1 = Random (1,11)
     reading->latitude = (int)esp_random()%181; //esp_random() needs to execute first
    reading->latitude -= -90; //Random (-90,90);
    reading->longitude = esp_random()%181;
    sprintf(reading->datetime,"DateTime UTC now");
    reading->intensity = esp_random()%101;
}

//struct size
void populateCanonicalReading(struct reading_t *reading)
{   
    ESP_LOGI(TAG_READING, "populating canonical Reading");
    reading->sensor_id = 1;
    reading->latitude -= -10;
    reading->longitude = 3;
    sprintf(reading->datetime,"DateTime UTC now");
    reading->intensity = 4;
}

void printReading(struct reading_t *reading)
{   
    ESP_LOGI(TAG_READING, "sensor_id = %d\t latitude = %f\t longitude = %f\t datetime = %s\t intensity = %d\t size of struct reading_t = %d bytes", reading->sensor_id, reading->latitude, reading->longitude, reading->datetime, reading->intensity, sizeof(struct reading_t));
}
 

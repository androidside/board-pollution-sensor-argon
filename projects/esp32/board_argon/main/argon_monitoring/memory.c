
#include <stdio.h>
#include <string.h>
#include "esp_heap_caps.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define TAG_MEMORY "MEMORY"
  /**************************************************/
  /******************* -  SRAM 1,2,3 - Tasks' RAM Allocation *******************/
  /**************************************************/


void printMemory(char *taskName, TaskHandle_t *taskHandle)
  {
    ESP_LOGI(TAG_MEMORY, "xPortGetFreeHeapSize %dk = DRAM", xPortGetFreeHeapSize());

    int DRam = heap_caps_get_free_size(MALLOC_CAP_8BIT);
    int IRam = heap_caps_get_free_size(MALLOC_CAP_32BIT) - heap_caps_get_free_size(MALLOC_CAP_8BIT);

    ESP_LOGI(TAG_MEMORY, "DRAM \t\t %d",DRam);
    ESP_LOGI(TAG_MEMORY, "IRAM \t\t %d",IRam);
    int free = heap_caps_get_largest_free_block(MALLOC_CAP_8BIT);
    ESP_LOGI(TAG_MEMORY, "largest block free = %d",free);
    // int stackmem = uxTaskGetStackHighWaterMark(NULL);
    // ESP_LOGI(TAG_MEMORY, "main stack space = %d", stackmem);
    int stackmem = uxTaskGetStackHighWaterMark(*taskHandle);
    ESP_LOGI(TAG_MEMORY, "\'%s\' task stack space = %d", taskName, stackmem);
  }
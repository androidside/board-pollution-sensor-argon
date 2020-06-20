
#include <stdio.h>
#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include <sys/dirent.h>
#include "esp_err.h"
#include "esp_log.h"
#include "esp_vfs_fat.h"
#include "freertos/task.h"
#include "driver/sdmmc_host.h"
#include "driver/sdspi_host.h"
#include "sdmmc_cmd.h"
#include "freertos/semphr.h"

static const char *TAG_SDCARD = "SD-CARD";

#define PIN_NUM_MISO 19
#define PIN_NUM_MOSI 23
#define PIN_NUM_CLK 18
#define PIN_NUM_CS 5
#define MAX_BUFSIZE 16384

extern xSemaphoreHandle mutexBus;

// void listDir(const char *dirName)
// {
//     DIR *dir = opendir(dirName);
//     if (dir == NULL)
//     {
//         ESP_LOGE(TAG_SDCARD, "cannot open dir %s", dirName);
//         return;
//     }
//     struct dirent *entry;
//     while ((entry = readdir(dir)) != NULL)
//     {
//         char fullPath[300];
//         sprintf(fullPath, "%s/%s", dirName, entry->d_name);
//         struct stat entryStat;
//         if (stat(fullPath, &entryStat) == -1)
//         {
//             ESP_LOGE(TAG_SDCARD, "error getting stats for %s", fullPath);
//         }
//         else
//         {
//             if (entry->d_type != DT_DIR)
//             {
//                 ESP_LOGI(TAG_SDCARD, "file full path = %s, file size = %ld", fullPath, entryStat.st_size);
//             }
//             else
//             {
//                 ESP_LOGI(TAG_SDCARD, "Dir full path = %s", fullPath );
//                 listDir(fullPath);
//             }
//         }
//     }
// }

void activateSDCard(void *params)
{
    while (true)
    {

        if (xSemaphoreTake(mutexBus, 2000 / portTICK_PERIOD_MS))
        {

            sdmmc_host_t host = SDSPI_HOST_DEFAULT();                      //Configures the various things for the virtual file system (frequency, etc)
            sdspi_slot_config_t slot_config = SDSPI_SLOT_CONFIG_DEFAULT(); //Brings back the structure that allows me to configure the various pins (MISO, MOSI, CLK)
            slot_config.gpio_miso = PIN_NUM_MISO;                          //Configure pins that we want to use
            slot_config.gpio_mosi = PIN_NUM_MOSI;
            slot_config.gpio_sck = PIN_NUM_CLK;
            slot_config.gpio_cs = PIN_NUM_CS;

            esp_vfs_fat_sdmmc_mount_config_t mount_config = {                                 //Configuration structure of how we want to mount our spiffs file system
                                                             .format_if_mount_failed = false, //if set to true, will format the SSD if mounting fails
                                                             .max_files = 5};                 //Max files we can open at the same time

            sdmmc_card_t *card;
            esp_err_t ret = esp_vfs_fat_sdmmc_mount("/sdcard", &host, &slot_config, &mount_config, &card); //we create a virtual file system

            if (ret != ESP_OK)
            {
                if (ret == ESP_FAIL)
                {
                    ESP_LOGE(TAG_SDCARD, "Failed to mount filesystem. "
                                  "If you want the card to be formatted, set format_if_mount_failed = true.");
                }
                else
                {
                    ESP_LOGE(TAG_SDCARD, "Failed to initialize the card (%d). " // If i receive this error there might be a wiring problem
                                  "Make sure SD card lines have pull-up resistors in place.",
                             ret);
                }
                xSemaphoreGive(mutexBus);
                vTaskDelay(10000 / portTICK_PERIOD_MS);
                continue;
            }

            // Card has been initialized, print its properties
            sdmmc_card_print_info(stdout, card);

            // Use POSIX and C standard library functions to work with files.
            // First create a file.
            ESP_LOGI(TAG_SDCARD, "Opening file");
            FILE *f = fopen("/sdcard/hello.txt", "w");
            if (f == NULL)
            {
                ESP_LOGE(TAG_SDCARD, "Failed to open file for writing");
                xSemaphoreGive(mutexBus);
                vTaskDelay(10000 / portTICK_PERIOD_MS);
                continue;
            }
            fprintf(f, "Hello %s!\n", card->cid.name);
            fclose(f);
            ESP_LOGI(TAG_SDCARD, "File written");

            // Check if destination file exists before renaming
            struct stat st;
            if (stat("/sdcard/foo.txt", &st) == 0)
            {
                // Delete it if it exists
                unlink("/sdcard/foo.txt");
            }

            // Rename original file
            ESP_LOGI(TAG_SDCARD, "Renaming file");
            if (rename("/sdcard/hello.txt", "/sdcard/foo.txt") != 0)
            {
                ESP_LOGE(TAG_SDCARD, "Rename failed");
                xSemaphoreGive(mutexBus);
                vTaskDelay(10000 / portTICK_PERIOD_MS);
                continue;
            }

            // Open renamed file for reading
            ESP_LOGI(TAG_SDCARD, "Reading file");
            f = fopen("/sdcard/foo.txt", "r");
            if (f == NULL)
            {
                ESP_LOGE(TAG_SDCARD, "Failed to open file for reading");
                xSemaphoreGive(mutexBus);
                vTaskDelay(10000 / portTICK_PERIOD_MS);
                continue;
            }
            char line[64];
            fgets(line, sizeof(line), f);
            fclose(f);
            // strip newline
            char *pos = strchr(line, '\n');
            if (pos)
            {
                *pos = '\0';
            }
            ESP_LOGI(TAG_SDCARD, "Read from file: '%s'", line);
         xSemaphoreGive(mutexBus);
        vTaskDelay(10000 / portTICK_PERIOD_MS);
        }

         else
        {
            ESP_LOGE(TAG_SDCARD, "sdcard.c: mutexBus timeout");
        }

       
    }
    //recurse through directories
    //listDir("/sdcard");
}
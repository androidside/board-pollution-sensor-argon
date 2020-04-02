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

void app_main()
{

  /**************************************************/
  /******************* -  Hall sensor - Touch Capacitive sensor *******************/
  /**************************************************/

// #include "driver/touch_pad.h"

  // void app_main()
  // {
  //   touch_pad_init();
  //   touch_pad_set_voltage(TOUCH_HVOLT_2V7, TOUCH_LVOLT_0V5, TOUCH_HVOLT_ATTEN_1V);
  //   touch_pad_config(TOUCH_PAD_GPIO13_CHANNEL, -1);
  //   touch_pad_filter_start(10);

  //   uint16_t val;
  //   uint16_t touch_filter_value = 0;
  //   uint16_t touch_rawValue = 0;

  //   while (true)
  //   {
  //     touch_pad_read_raw_data(TOUCH_PAD_GPIO13_CHANNEL, &touch_rawValue);
  //     touch_pad_read_filtered(TOUCH_PAD_GPIO13_CHANNEL, &touch_filter_value);
  //     touch_pad_read(TOUCH_PAD_GPIO13_CHANNEL, &val);
  //     printf("val = %d raw = %d filtered = %d\n", val, touch_rawValue, touch_filter_value);
  //     vTaskDelay(1000 / portTICK_PERIOD_MS);
  //   }
    /**************************************************/
    /******************* -  Hall sensor - Magnetic radiation *******************/
    /**************************************************/
    // uint32_t hall_sens_read();

    // void app_main(void)
    // {
    //   while (1)
    //   {
    //     printf("Hall Sens: %d\n", hall_sens_read());
    //     vTaskDelay(100 / portTICK_PERIOD_MS);
    //   }
    // }

    /**************************************************/
    /******************* -  PWM LED Blink  *******************/
    /**************************************************/

    // void app_main(void)
    // {
    //   ledc_timer_config_t timer = {
    //       .speed_mode = LEDC_LOW_SPEED_MODE,
    //       .duty_resolution = LEDC_TIMER_10_BIT,
    //       .timer_num = LEDC_TIMER_0,
    //       .freq_hz = 5000,
    //       .clk_cfg = LEDC_AUTO_CLK};

    //   ledc_timer_config(&timer);

    //   ledc_channel_config_t channel = {
    //       .gpio_num = 4,
    //       .speed_mode = LEDC_LOW_SPEED_MODE,
    //       .channel = LEDC_CHANNEL_0,
    //       .timer_sel = LEDC_TIMER_0,
    //       .duty = 0,
    //       .hpoint = 0};
    //   ledc_channel_config(&channel);

    //   ledc_fade_func_install(0);
    //   for (int i = 0; i < 1024; i++)
    //   {
    //     ledc_set_duty_and_update(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, i, 0);
    //     // ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, i);
    //     // ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
    //     vTaskDelay(10 / portTICK_PERIOD_MS);
    //   }

    //   while (true)
    //   {
    //     ledc_set_fade_time_and_start(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 0, 100, LEDC_FADE_WAIT_DONE);
    //     ledc_set_fade_time_and_start(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 1024, 100, LEDC_FADE_WAIT_DONE);
    //   }

    /**************************************************/
    /******************* -  DAC and ADC Interrupt  *******************/
    /**************************************************/

    //  adc1_config_width(ADC_WIDTH_BIT_12);
    //  adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_DB_0);
    //  while (true)
    //  {
    //      int val = adc1_get_raw(ADC1_CHANNEL_0);
    //      printf("value is %d\n", val);
    //      vTaskDelay(1000 / portTICK_PERIOD_MS);
    //  }
    // dac_output_enable(DAC_CHANNEL_1);
    // dac_output_voltage(DAC_CHANNEL_1, 127);

    /**************************************************/
    /******************* -  GPIO Interrupt / Bouncing/ Debouncing  *******************/
    /**************************************************/

    // #define PIN_SWITCH 15
    // #define PIN_LED 25

    // xQueueHandle interruptQueue;

    // static void IRAM_ATTR gpio_isr_handler(void *args)
    // { //code run on dynamic RAM
    //   int pinNumber = (int)args;
    //   xQueueSendFromISR(interruptQueue, &pinNumber, NULL);
    // }

    // void buttonPushedTask(void *params)
    // {

    //   int pinNumber, count = 0;
    //   while (true)
    //   {
    //     if (xQueueReceive(interruptQueue, &pinNumber, portMAX_DELAY))
    //     {
    //       //De-Bouncing !!!

    //       // disable the interrupt
    //       gpio_isr_handler_remove(pinNumber);

    //       // wait some time while we check for the button to be released
    //       do
    //       {
    //         vTaskDelay(50 / portTICK_PERIOD_MS);
    //       } while (gpio_get_level(pinNumber) == 1);

    //       //do some work
    //       printf("GPIO %d was pressed %d times. The state is %d\n", pinNumber, count++, gpio_get_level(PIN_SWITCH));

    //       // re-enable the interrupt
    //       gpio_isr_handler_add(pinNumber, gpio_isr_handler, (void *)pinNumber);

    //     }
    //   }
    // }

    // void app_main()
    // {
    //   gpio_pad_select_gpio(PIN_LED);
    //   gpio_set_direction(PIN_LED, GPIO_MODE_OUTPUT);
    //   gpio_pulldown_en(PIN_LED);
    //   // gpio_pullup_en(PIN_LED);

    //   // gpio_pad_select_gpio(PIN_SWITCH);
    //   // gpio_set_direction(PIN_SWITCH, GPIO_MODE_INPUT);
    //   // gpio_pulldown_en(PIN_SWITCH);
    //   // gpio_pullup_dis(PIN_SWITCH);
    //   // gpio_set_intr_type(PIN_SWITCH, GPIO_INTR_POSEDGE); //Wake up on a positive edge

    //   gpio_config_t config;
    //   config.intr_type = GPIO_INTR_POSEDGE;
    //   config.mode = GPIO_MODE_INPUT;
    //   config.pull_down_en = 1;
    //   config.pull_up_en = 0;
    //   config.pin_bit_mask = (1ULL<<PIN_SWITCH); //1 Long, Long shifted PIN_SWITCH positions YOU CAN | (1ULL<<5), ETC
    //   gpio_config(&config);

    //   interruptQueue = xQueueCreate(10, sizeof(int));
    //   xTaskCreate(buttonPushedTask, "buttonPushedTask", 2048, NULL, 1, NULL);

    //   gpio_install_isr_service(0);
    //   gpio_isr_handler_add(PIN_SWITCH, gpio_isr_handler, (void *)PIN_SWITCH);

    /**************************************************/
    /******************* -  GPIO Input/Output basic LED Button example *******************/
    /**************************************************/

    // #define PIN_SWITCH 12
    // #define PIN_LED 32

    //   void app_main()
    //   {
    //     gpio_pad_select_gpio(PIN_LED);
    //     gpio_set_direction(PIN_LED, GPIO_MODE_OUTPUT);
    //     gpio_pulldown_en(PIN_LED);
    //     // gpio_pullup_en(PIN_LED);

    //     gpio_pad_select_gpio(PIN_SWITCH);
    //     gpio_set_direction(PIN_SWITCH, GPIO_MODE_INPUT);
    //     gpio_pulldown_en(PIN_SWITCH);
    //     gpio_pullup_dis(PIN_SWITCH);
    //     // gpio_pullup_en(PIN_SWITCH);

    //     while (true)
    //     {
    //       int level = gpio_get_level(PIN_SWITCH);
    //       // printf("Level = %d\n",level);
    //       gpio_set_level(PIN_LED, level);
    //       // vTaskDelay(1000/portTICK_PERIOD_MS);
    //       vTaskDelay(1);
    //     }

    /**************************************************/
    /******************* -  SPIFFS Store entire directories and loop through directories *******************/
    /**************************************************/

    // #define TAG "SPIFFS"

    //   void app_main()
    //   {
    //     esp_vfs_spiffs_conf_t config = {
    //         .base_path = "/spiffs",
    //         .partition_label = NULL,
    //         .max_files = 5,
    //         .format_if_mount_failed = true,
    //     };

    //     esp_vfs_spiffs_register(&config);

    //     DIR *dir = opendir("/spiffs");

    //     struct dirent *entry;
    //     while ((entry = readdir(dir)) != NULL)
    //     {
    //       char fullPath[300];
    //       sprintf(fullPath, "/spiffs/%s", entry->d_name); //entry->d_name gives us the path without the spiffs
    //       struct stat entryStat;
    //       if (stat(fullPath, &entryStat) == 1)
    //       {
    //         ESP_LOGE(TAG, "error getting stats for %s", fullPath);
    //       }
    //       else
    //       {
    //         ESP_LOGI(TAG, "full path = %s, file size = %ld", fullPath, entryStat.st_size);
    //       }
    //     }
    //     size_t total = 0, used = 0;
    //     esp_spiffs_info(NULL, &total, &used);
    //     ESP_LOGI(TAG, "total = %d, used = %d", total, used);

    //     FILE *file = fopen("/spiffs/index.html", "r");
    //     if (file == NULL)
    //     {
    //       ESP_LOGE(TAG, "could not open the file");
    //     }
    //     else
    //     {
    //       char line[256];
    //       while (fgets(line, sizeof(line), file) != NULL)
    //       {
    //         printf(line);
    //       }

    //       fclose(file);
    //     }
    //     esp_vfs_spiffs_unregister(NULL); //the first partition can be called NULL!

    /**************************************************/
    /******************* -  NVS Storage Cats *******************/
    /**************************************************/

    // #define TAG "NVS"

    // typedef struct cat_struct
    // {
    //   char name[20];
    //   int age;
    //   int id;
    // } Cat;

    // void app_main()
    // {
    //   vTaskDelay(1000 / portTICK_PERIOD_MS);

    //   ESP_ERROR_CHECK(nvs_flash_init_partition("MyNvs"));

    //   nvs_handle handle;
    //   ESP_ERROR_CHECK(nvs_open_from_partition("MyNvs", "cat_store", NVS_READWRITE, &handle));

    //   char catKey[15];
    //   Cat cat;
    //   size_t catSize;
    //   for (int i = 0; i < 5; i++)
    //   {
    //     int n=sprintf(catKey, "ca_%d", i);
    //     esp_err_t result = nvs_get_blob(handle, catKey, (void *)&cat, &catSize);

    //     // nvs_stats_t nvsStats;
    //     // nvs_get_stats("MyNvs", &nvsStats);
    //     // ESP_LOGI(TAG, "used: %d, free: %d, total: %d, namespace count: %d", nvsStats.used_entries,
    //     //          nvsStats.free_entries, nvsStats.total_entries, nvsStats.namespace_count);

    //     // int32_t val = 0;
    //     // esp_err_t result = nvs_get_i32(handle, "val", &val);
    //     switch (result)
    //     {
    //     case ESP_ERR_NOT_FOUND:
    //     case ESP_ERR_NVS_NOT_FOUND:
    //       ESP_LOGE(TAG, "Value not set yet");
    //       break;
    //     case ESP_OK:
    //       ESP_LOGI(TAG, "cat name: %s, age %d, id %d", cat.name, cat.age, cat.id);
    //       break;
    //     default:
    //       ESP_LOGE(TAG, "Error (%s) opening NVS handle!\n", esp_err_to_name(result));
    //       break;
    //     }
    //   }

    //   for (int i = 0; i < 5; i++)
    //   {
    //     int n=sprintf(catKey, "ca_%d", i);
    //     Cat newCat;
    //     sprintf(newCat.name, "Mr Cat %d",i);
    //     newCat.age = i+2;
    //     newCat.id = i;
    //     ESP_ERROR_CHECK(nvs_set_blob(handle, catKey, (void *) &newCat, sizeof(Cat)));
    //     ESP_ERROR_CHECK(nvs_commit(handle));
    //   }
    //     nvs_close(handle);

    /**************************************************/
    /******************* -  Flashing Files into Memory *******************/
    /**************************************************/

    // extern const unsigned char indexHtml[] asm("_binary_index_html_start");
    // printf("html = %s\n", indexHtml);

    // extern const unsigned char sample[] asm("_binary_sample_txt_start");
    // printf("sample = %s\n", sample);

    // extern const unsigned char imgStart[] asm("_binary_pinout_jpg_start");
    // extern const unsigned char imgEnd[] asm("_binary_pinout_jpg_end");
    // const unsigned int imgSize = imgEnd - imgStart;
    // printf("img size = %d\n", imgSize);

    /**************************************************/
    /******************* -  SRAM 1,2,3 - Tasks' RAM Allocation *******************/
    /**************************************************/

    // #define TAG "Memory"

    // void aTask(void *param){

    //   int stackmem = uxTaskGetStackHighWaterMark(NULL);
    //   ESP_LOGI(TAG, "atask stack space = %d", stackmem);

    //   char buffer[6500];
    //   memset(&buffer, 1, sizeof(buffer));

    //   while (true)
    //   {
    //     vTaskDelay(1000/portTICK_PERIOD_MS);
    //   }

    // }

    // void app_main(void)
    // {
    //   ESP_LOGI(TAG, "xPortGetFreeHeapSize %dk = DRAM", xPortGetFreeHeapSize());

    //   int DRam = heap_caps_get_free_size(MALLOC_CAP_8BIT);
    //   int IRam = heap_caps_get_free_size(MALLOC_CAP_32BIT) - heap_caps_get_free_size(MALLOC_CAP_8BIT);

    //   ESP_LOGI(TAG, "DRAM \t\t %d",DRam);
    //   ESP_LOGI(TAG, "IRAM \t\t %d",IRam);
    //   int free = heap_caps_get_largest_free_block(MALLOC_CAP_8BIT);
    //   ESP_LOGI(TAG, "free = %d",free);
    //   int stackmem = uxTaskGetStackHighWaterMark(NULL);
    //   ESP_LOGI(TAG, "main stack space = %d", stackmem);
    //   xTaskCreate(&aTask, "a task", 8000, NULL, 1, NULL);

    /**************************************************/
    /******************* -  Menu Config -  *******************/
    /**************************************************/

    // #define TAG "CONFIG"

    // void app_main(void)
    // {
    //   ESP_LOGI(TAG, "MY INT %d", CONFIG_MY_INT);
    //   ESP_LOGI(TAG, "MY STRING %s", CONFIG_MY_STRING);

    //   bool my_bool = false;

    //   #ifdef CONFIG_MY_BOOL
    //     my_bool = true;
    //   #else
    //     my_bool = false;
    //   #endif
    //   ESP_LOGI(TAG, "MY_BOOL %s", my_bool? "yes" : "no");

    //   int option = 0;

    //   #ifdef CONFIG_OPTION_1 //aka: IF THAT IS INSIDE OUR DEFINITIONS FILE ...
    //     option = 1;
    //   #elif CONFIG_OPTION_2
    //     option = 2;
    //   #else
    //     option = 3;
    //   #endif

    //   ESP_LOGI(TAG, "MY_OPTION %d", option);

    /**************************************************/
    /******************* -  Event Groups -  *******************/
    /**************************************************/

    /* If I receive an HTTP Request AND a Bluetoth Message, I trigger task1 */

    //   EventGroupHandle_t eventGroup;
    // const int gotHTTP = BIT0; // (1<<0) 1 BIT shited left 1 position
    // const int gotBLE =  BIT1;

    // void listenForHTTP(void *params)
    // {
    //   int count = 0;
    //   while (true)
    //   {
    //     xEventGroupSetBits(eventGroup, gotHTTP);
    //     printf("Got an HTTP request\n");
    //     vTaskDelay(2000 / portTICK_PERIOD_MS);
    //   }
    // }
    // void listenForBluetooth(void *params)
    // {

    //   while (true)
    //   {
    //     xEventGroupSetBits(eventGroup, gotBLE);
    //     printf("got BLE\n");
    //     vTaskDelay(2000 / portTICK_PERIOD_MS);
    //   }
    // }
    // void task1(void *params)
    // {
    //   while (true)
    //   {
    //     xEventGroupWaitBits(eventGroup, gotHTTP | gotBLE, true, true, portMAX_DELAY); //1st true will clear the bits! 2nd true: wiat for all of them
    //     printf("received HTTP and BLE\n");
    //   }
    // }

    // void app_main(void)
    // {
    //   eventGroup = xEventGroupCreate();
    //   xTaskCreate(&listenForHTTP, "get http", 2048, NULL, 1, NULL);
    //   xTaskCreate(&listenForBluetooth, "get Bluetooth", 2048, NULL, 1, NULL);
    //   xTaskCreate(&task1, "do something with http", 2048, NULL, 1, NULL);

    /**************************************************/
    /******************* -  Queues -  *******************/
    /**************************************************/

    // xQueueHandle queue;

    // void listenForHTTP(void *params)
    // {
    //   int count = 0 ;
    //   while (true)
    //   {
    //     count++;
    //     printf("Received Message\n");
    //     long ok = xQueueSend(queue, &count, 1000/portTICK_PERIOD_MS );
    //     if (ok){
    //       printf("added message to queue\n");
    //     }
    //     else{
    //       printf("failed to add message to queue\n");
    //     }
    //       vTaskDelay(1000 / portTICK_PERIOD_MS);
    //   }
    // }

    // void task1(void *params)
    // {
    //   while (true)
    //   {
    //     int rxInt;
    //     if(xQueueReceive(queue, &rxInt, 5000/portTICK_PERIOD_MS))
    //     {
    //       printf("Doing something with http %d\n", rxInt);
    //     }
    //     else{
    //       printf("Failed to received\n");
    //     }

    //   }
    // }

    // void app_main(void)
    // {
    //   queue = xQueueCreate(3, sizeof(int));
    //   xTaskCreate(&listenForHTTP, "get http", 2048, NULL, 1, NULL);
    //   xTaskCreate(&task1, "do something with http", 2048, NULL, 1, NULL);

    /**************************************************/
    /******************* -  Binary Semaphore -  *******************/
    /**************************************************/

    // xSemaphoreHandle binSemaphore;

    // void listenForHTTP(void *params)
    // {
    //   while (true)
    //   {
    //     printf("Received Message\n");
    //     xSemaphoreGive(binSemaphore);
    //     printf("Processing HTTP message\n");
    //     vTaskDelay(5000 / portTICK_PERIOD_MS);
    //   }
    // }

    // void task1(void *params)
    // {
    //   while (true)
    //   {
    //     xSemaphoreTake(binSemaphore, portMAX_DELAY); //NEVER TIMES OUT!
    //     printf("Doing something with http\n");
    //   }
    // }

    // void app_main(void)
    // {
    //   binSemaphore = xSemaphoreCreateBinary();
    //   xTaskCreate(&listenForHTTP, "get http", 2048, NULL, 2, NULL);
    //   xTaskCreate(&task1, "do something with http", 2048, NULL, 1, NULL);

    /**************************************************/
    /******************* -  Mutex -  *******************/
    /**************************************************/

    //     xSemaphoreHandle mutexBus;

    // void writeToBus(char *message) //ie: WRITING AN I2C bus, precious resource
    // {
    //   printf(message);
    // }

    // void task1(void *params)
    // {
    //   while (true)
    //   {
    //     printf("Reading temperature\n");
    //     if (xSemaphoreTake(mutexBus, 1000 / portTICK_PERIOD_MS))
    //     {
    //       writeToBus("Temperature is 25C\n");
    //       xSemaphoreGive(mutexBus);
    //     }
    //     else
    //     {
    //       printf("Reading temperature timed out\n");
    //     }
    //     vTaskDelay(1000 / portTICK_PERIOD_MS);
    //   }
    // }

    // void task2(void *params)
    // {
    //   while (true)
    //   {
    //     printf("Reading humidity\n");
    //     if (xSemaphoreTake(mutexBus, 1000 / portTICK_PERIOD_MS))
    //     {
    //       writeToBus("Humidity is 50\%\n");
    //       xSemaphoreGive(mutexBus);

    //     }
    //     else
    //     {
    //       printf("Reading humidity timed out\n");
    //     }
    //     vTaskDelay(2000 / portTICK_PERIOD_MS);

    //   }
    // }
    //   void app_main(void)
    //   {
    //     mutexBus = xSemaphoreCreateMutex();
    //     xTaskCreate(&task1, "Temperature reading", 2048, NULL, 2, NULL);
    //     xTaskCreate(&task2, "Humidity reading", 2048, NULL, 2, NULL);

    /**************************************************/
    /******************* -  Task Notification Sender&Receiver and sending data between tasks -  *******************/
    /**************************************************/

    // static TaskHandle_t receiverHandler = NULL;

    // void sender(void *params){
    //   while(true){
    //     xTaskNotify(receiverHandler, (1<<0), eSetValueWithOverwrite);
    //     vTaskDelay(1000 / portTICK_PERIOD_MS);
    //     xTaskNotify(receiverHandler, (1<<1), eSetValueWithOverwrite);
    //     vTaskDelay(1000 / portTICK_PERIOD_MS);
    //     xTaskNotify(receiverHandler, (1<<2), eSetValueWithOverwrite);
    //     vTaskDelay(1000 / portTICK_PERIOD_MS);
    //     xTaskNotify(receiverHandler, (1<<3), eSetValueWithOverwrite);
    //     vTaskDelay(1000 / portTICK_PERIOD_MS);
    //   }

    // }

    // void receiver(void *params)
    // {
    //  uint32_t state;
    //  while(true){
    //    xTaskNotifyWait(0,0,&state, portMAX_DELAY);
    //    printf("received state %d times\n", state);
    //   }

    // }

    // void app_main(void)
    // {
    //   xTaskCreate(&receiver, "sender" , 2048, NULL, 2, &receiverHandler);
    //   xTaskCreate(&sender, "receiver" , 2048, NULL, 2, NULL);

    /**************************************************/
    /******************* - Declare Tasks to use any core -  *******************/
    /**************************************************/

    // void task1(void *params){
    //   while(true){
    //     printf("reading temperature from %s\n", (char*) params);
    //     vTaskDelay(1000 / portTICK_PERIOD_MS);
    //   }

    // }

    // void task2(void *params){
    //  while(true){
    //     printf("reading humidty from %s\n", (char*) params);
    //     vTaskDelay(1000 / portTICK_PERIOD_MS);
    //   }

    // }

    // void app_main(void)
    // {
    //   xTaskCreate(&task1, "temperature reading" , 2048, "task 1", 2, NULL);
    //   xTaskCreatePinnedToCore(&task2, "humidity reading" , 2048, "task 2", 2, NULL,1);

    /**************************************************/
    /******************* - Header files -  *******************/
    /**************************************************/

    // #include "module_jordi.h"
    // moduleFunction(4,9);
    // printf("Result is = %d\n", result);

    /**************************************************/
    /******************* - Function Pointers -  *******************/
    /**************************************************/

    // typedef struct person_t{
    // char firstName[20];
    // char lastName[20];
    // int age;
    // void (*DoWork)(char *dataRetrieved)
    // }person_t;

    // void updatePerson(person_t *person, char *dataRetrieved){
    //   strcpy(person->firstName, "From Data Retrieved");
    //   strcpy(&person->lastName, "From Data Retrieved");
    //   person->DoWork=doWorkForPerson;
    //   person->DoWork("some params");
    //   person->age =  29;
    //   }

    // void connectAndGetInfo(char * url,void (*DoWork)(char *dataRetrieved))
    // //function pointer needs to specify what type of data is expecting on the call!
    // {
    //   //connect to wireless
    //   // connect to endpoint
    //   // allocate memory
    //   char *dataRetrieved = (char *)malloc(1024);
    //   // fill buffer with data: getRequest(&dataRetrieved);
    //   // do work and get a person object
    //   DoWork(dataRetrieved);
    //   // clean up memory and other resources
    //   free((void *)dataRetrieved);
    // }

    // void doWorkForPerson(char *dataRetrieved)
    // {
    //   person_t person;
    //   updatePerson(&person, *dataRetrieved);
    //   printf("My name is %s %s and I am %d years old\n",person.firstName, person.lastName, person.age);
    // }

    // void app_main(void)
    // {
    //   person_t person;
    //   connectAndGetInfo("http://getperson.com", doWorkForPerson);

    /**************************************************/
    /******************* - Structure and Pointer -  *******************/
    /**************************************************/

    // typedef struct person_t{
    // char firstName[20];
    // char lastName[20];
    // int age;
    // }person_t;

    // void updatePerson(person_t *person, char firstName[20], char lastName[20], int age){
    //   strcpy(person->firstName, firstName);
    //   strcpy(&person->lastName, lastName);
    //   person->age =  29;
    //   }

    //   void exclamIt(char *phrase){
    //     strcat(phrase,"!");
    //   }

    // void app_main(void)
    // {
    //   person_t person;
    //   char phrase[20] = "Hello World";
    //   exclamIt(phrase);
    //   printf("%s\n",phrase);
    //   updatePerson(&person, "Bobby", "Fisher", 29);

    //   printf("My name is %s %s and I am %d years old\n",person.firstName, person.lastName, person.age);

    // }

    /**************************************************/
    /******************* - Keyboard input -  *******************/
    /**************************************************/
    // char c = 0;
    // char str[100];
    // memset(str,0,sizeof(str));
    // while(c!='\n')
    // {
    //   c = getchar(); //returns an integer! NOT A BLOCKING FUNCTION, if no character is found, it's gonna be 0xff
    //   if(c != 0xff)
    //   {
    //     str[strlen(str)] = c;
    //     printf("%c", c);
    //   }

    //   vTaskDelay(100 / portTICK_PERIOD_MS); // so the watchdog timer doesn't get upset with us sitting in an infinite loop
    // }

    // printf("You typed: %s\n", str);

    /**************************************************/
    /******************* - Blinkey -  *******************/
    /**************************************************/

    // #include "freertos/FreeRTOS.h"
    // #include "freertos/task.h"
    // #include "esp_system.h"
    // #include "driver/gpio.h"
    // #define PIN 2

    // gpio_pad_select_gpio(PIN);
    // gpio_set_direction(PIN, GPIO_MODE_OUTPUT);

    // while (true)
    // {
    //   gpio_set_level(PIN, 1);
    //   vTaskDelay(1000 / portTICK_PERIOD_MS);
    //   gpio_set_level(PIN, 0);
    //   vTaskDelay(1000 / portTICK_PERIOD_MS);
    //   printf("Hello World\n");
    // }

    /**************************************************/
    /******************* - Random -  *******************/
    /**************************************************/

    // #define TAG "DICE"

    // int dice_role()
    // {
    //   int random = esp_random();
    //   int positiveNumber = abs(random);
    //   int diceNumber = (positiveNumber % 6) + 1;
    //   return diceNumber;
    //}

    // int i = 0;
    // while (true)
    // {
    // vTaskDelay(1000/ portTICK_PERIOD_MS);
    // ESP_LOGI(TAG, "random number %d", dice_role());

    // }

    /**************************************************/
    /******************* - TASK DELAY -  *******************/
    /**************************************************/
    // #define TAG "DELAY"
    // int i = 0;
    // while (true)
    // {
    // vTaskDelay(1000/ portTICK_PERIOD_MS);
    // ESP_LOGI(TAG, "in loop %d", i++);

    // }

    /**************************************************/
    /******************* - LOGS -  *******************/
    /**************************************************/

    // printf("Hello world!\n");
    // esp_log_level_set("LOG", ESP_LOG_INFO);
    // ESP_LOGE("LOG", "This is an error" );
    // ESP_LOGW("LOG", "This is a warning" );
    // ESP_LOGI("LOG", "This is an info" );
    // ESP_LOGD("LOG", "This is Debug" );
    // ESP_LOGV("LOG", "This is Verbose" );

    // int number = 0 ;

    // ESP_LOGE("TAG2", "This is an error %d", number++ );

    // ESP_LOGW("TAG2", "This is a warning %d", number++ );
    // ESP_LOGI("TAG2", "This is an info %d", number++ );
    // ESP_LOGD("TAG2", "This is Debug %d", number++ );
    // ESP_LOGV("TAG2", "This is Verbose %d", number++ );
  }

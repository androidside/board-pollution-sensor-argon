#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <string.h>
#include "freertos/semphr.h"
#include "freertos/event_groups.h"


void app_main(void)
{
  


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


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
#include "protocol_examples_common.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "esp_http_client.h"
#include "argon_comms/fetch.h"
#include "cJSON.h"
#include "argon_comms/connect.h"
#include "argon_reading/reading.h"
#include "argon_monitoring/memory.h"
#include "argon_comms/connect.h"
#include "argon_reading/lm75a.h"
#include "argon_reading/no2.h"
#include "argon_reading/gpsapi.h"

#define TAG "TASKS"
#define MAX_APs 20
#define SSID CONFIG_WIFI_SSID
#define PASSWORD CONFIG_WIFI_PASSWORD

xQueueHandle queue;
static TaskHandle_t readSensorsTaskHandle;
static TaskHandle_t onConnectedTaskHandle;
xSemaphoreHandle onConnectionHandler;

void readSensors(void *params)
{
  struct reading_t reading;
  int readSensorsLoopCounter = 0;
  while (true)
  {
    printMemory("readSensor", &readSensorsTaskHandle);
    populateDemoReadingRandom(&reading);
    ESP_LOGI(TAG, "Received Message");
    long ok = xQueueSend(queue, &reading, 1000 / portTICK_PERIOD_MS);
    if (ok)
    {
      ESP_LOGI(TAG, "readSensors(): message added to queue");
    }
    else
    {
      ESP_LOGE(TAG, "readSensors(): failed to add message to queue, saving in memory");
    }
    vTaskDelay(10000 / portTICK_PERIOD_MS);
    ESP_LOGI(TAG, "loopCounter_readSensors = %d", readSensorsLoopCounter++);
  }
}


void onConnected(void *params)
{
  struct reading_t reading;
  int onConnectedLoopCounter = 0;
  while (true)
  {
    //It won't proceed any further until we get an ip address
    if (xSemaphoreTake(onConnectionHandler, 15 * 1000 / portTICK_PERIOD_MS)) //We wait for 15 seconds max
    {
      //printMemory("onConnected", &onConnectedTaskHandle);
      if (xQueueReceive(queue, &reading, 5000 / portTICK_PERIOD_MS))
      {
        ESP_LOGI(TAG, "onConnected(): Message received sending over HTTP");
        if(postReading(&reading) == 200){

          ESP_LOGI(TAG, "onConnected(): Reading Posted !!");          
          //Send all readings in memory

        }
        else{
            ESP_LOGE(TAG, "onConnected(): Reading Failed, saving to memory !!!");      
        }     

      }
      else
      {
        ESP_LOGE(TAG, "onConnected(): Failed to receive message");
      }
     // ESP_LOGI(TAG, "loopCounter_onConnected = %d", onConnectedLoopCounter++);
    }
    else
    {
      //Wifi connection not sucessful
       ESP_LOGE(TAG, "Failed to connect. Retry in");
        for (int i = 0; i < 5; i++)
        {
          ESP_LOGI(TAG, "...%d", i);
          vTaskDelay(1000 / portTICK_RATE_MS);
        }
        esp_restart();
        //the while loop will execute again
    }
    //We will take the semaphore again and continue
    xSemaphoreGive(onConnectionHandler);
  }
}

void app_main()
{
  xTaskCreate(&activateGPS, "read gps", 1024 * 8, NULL, 5, NULL);
  vTaskDelay(5000 / portTICK_RATE_MS);

  onConnectionHandler = xSemaphoreCreateBinary();
  wifiInit();
  queue = xQueueCreate(3, sizeof(struct reading_t));
  xTaskCreate(&readSensors, "create reading", 1024 * 8, NULL, 1, &readSensorsTaskHandle);
  xTaskCreate(&onConnected, "send reading over HTTP", 1024 * 10, NULL, 1, &onConnectedTaskHandle);
  xTaskCreate(&activateLM75A, "read temperature", 1024 *4, NULL, 4, NULL);
  xTaskCreate(&activateNO2, "read no2", 1024 *3, NULL, 5, NULL);
  




  /**************************************************/
  /******************* -  GET and POST request quote of the day BOILER Plate FINAL VERSION *******************/
  /**************************************************/

//   #define TAG "DATA"
//   #define NUMBER "655807482"

//   xSemaphoreHandle onConnectionHandler;
//   int testextern;

// //Specific OnGotData functio used to parse quotes from the GET request
//   void OnGotData(char *incomingBuffer, char *output)
//   {
//     ESP_LOGI(TAG, "Parsing quote");
//     //that gives the entire payload from bracket to bracket
//     cJSON *payload = cJSON_Parse(incomingBuffer);
//     //now we have to get the contents key
//     cJSON *contents = cJSON_GetObjectItem(payload, "contents");
//     cJSON *quotes = cJSON_GetObjectItem(contents, "quotes");
//     cJSON *quotesElement;
//     //Sometimes the content can be an array of quotes, so we have to iterate over the quotes array
//     cJSON_ArrayForEach(quotesElement, quotes)
//     {
//       cJSON *quote = cJSON_GetObjectItem(quotesElement, "quote");
//       ESP_LOGI(TAG, "Printing Final Quote");
//       ESP_LOGI(TAG, "%s", quote->valuestring);
//       strcpy(output, quote->valuestring);
//     }
//     //clean up the parent element and the child elements get cleaned automatically
//     cJSON_Delete(payload);
//   }

//   //3 parameters, number, message out to create the string
//   // void createBody(char *number, char *message, char *outputString){
//   //   //Add JSON structure to the string, we can use cJSON (look at documentation) or we can do it as follows
//   void createBody(char *number, char *message, char *outputString)
//   {
//     //I have to escape the quotes
//     sprintf(outputString,
//             "{"
//             "  \"messages\": ["
//             "      {"
//             "      "
//             "          \"content\": \"%s\","
//             "          \"destination_number\": \"%s\","
//             "          \"format\": \"SMS\""
//             "      }"
//             "  ]"
//             "}",
//             message, number);
//             //I need to put each individual line in an individual quote "line 1"
//     ESP_LOGI(TAG, "HTTP POST Body String = %s", outputString);

//   }

//   void onConnected(void *param)
//   {
//     while (true)
//     {
//       //It won't proceed any further until we get an ip address
//       if (xSemaphoreTake(onConnectionHandler, 10 * 1000 / portTICK_PERIOD_MS)) //We wait for 10 seconds
//       {
//         ESP_LOGI(TAG, "Processing");
//         struct fetchParams_t fetchParams;
//         fetchParams.OnGotData = OnGotData;
//         fetchParams.body = NULL;
//         fetchParams.headerCount = 0;
//         fetchParams.method = GET;
//         //do something useful and wait forever
//         fetch("https://quotes.rest/qod?language=en", &fetchParams);
//         //received ok, so we send sms
//         if (fetchParams.status == 200)
//         {
//           //send SMS
//           struct fetchParams_t smsStruct;
//           smsStruct.OnGotData = NULL;
//           smsStruct.OnGotData = NULL;
//           smsStruct.method = POST;

//           header_t headerContentType = {
//               .key = "Content-Type",
//               .val = "application/json"};

//           header_t headerAccept = {
//               .key = "Accept",
//               .val = "application/json"};

//           header_t headerAuthorization = {
//               .key = "Authorization",
//               .val = "Basic a3NScUxxOUZCeU9PbmVHVlJBSzA6aG5VMFJ5STVWcDJiRktSQWtIZEs5NmR6VnIzeTE3"};

//           smsStruct.header[0] = headerAuthorization;
//           smsStruct.header[1] = headerAccept;
//           smsStruct.header[2] = headerContentType;
//           smsStruct.headerCount = 3;

//           //Output buffer, I could use malloc()...
//           char buffer[1024];
//           //Populate the body of the message, we create a method
//           createBody(NUMBER, fetchParams.message, buffer);
//           smsStruct.body = buffer;
//           //We update out fetch.c to acomodate the new parameters
//           fetch("https://api.messagemedia.com/v1/messages", &smsStruct);

//         }
//         ESP_LOGI(TAG, "%s", fetchParams.message);
//         ESP_LOGI(TAG, "Done!");
//         //Disconnect from the internet, switch off wifi
//         esp_wifi_disconnect();
//         //Stop the execution of the program, wait forever so it doesn't get the information from the internet again
//         xSemaphoreTake(onConnectionHandler, portMAX_DELAY);
//       }

//       else
//       {
//         ESP_LOGE(TAG, "Failed to connect. Retry in");
//         for (int i = 0; i < 5; i++)
//         {
//           ESP_LOGI(TAG, "...%d", i);
//           vTaskDelay(1000 / portTICK_RATE_MS);
//         }
//         esp_restart();
//         //the while loop will execute again
//       }
//     }
//   }

//   void app_main()
//   {
//     onConnectionHandler = xSemaphoreCreateBinary();
//     wifiInit();
//     //1024 words * 5
//     xTaskCreate(&onConnected, "On Connected", 1024 * 5, NULL, 5, NULL);

/**************************************************/
/******************* -  Wifi Connect to the Internet BOILERPLATE CODE *******************/
/**************************************************/
// #define MAX_APs 20
// #define SSID CONFIG_WIFI_SSID
// #define PASSWORD CONFIG_WIFI_PASSWORD

// xSemaphoreHandle onConnectionHandler;

  //Method used on the esp_event_loop_init() function on the wifiInit function
  //This method captures/intercepts the states of the cycles that happen when we connect to the internet
  // static esp_err_t event_handler(void *ctx, system_event_t *event)
  // {
  //   switch (event->event_id)
  //   {
  //   case SYSTEM_EVENT_STA_START:
  //     esp_wifi_connect();
  //     printf("connecting ...\n");
  //     break;

  //   case SYSTEM_EVENT_STA_CONNECTED:
  //     printf("connected...\n");
  //     break;

  //   case SYSTEM_EVENT_STA_GOT_IP:
  //     printf("got ip...\n");
  //     //Gives me back the amount of space left on the stack
  //     printf("stack space is %d\n", uxTaskGetStackHighWaterMark(NULL));
  //     //We give the seamaphore so the task can continue
  //     xSemaphoreGive(onConnectionHandler);
  //     break;

  //   case SYSTEM_EVENT_STA_DISCONNECTED:
  //     printf("disconnected...\n");
  //     break;

  //   default:
  //     break;
  //   }
  //   return ESP_OK;
  // }

  // void wifiInit()
  // {
  //   //Initialize Non volatile storage NVS
  //   ESP_ERROR_CHECK(nvs_flash_init());
  //   //Initialize my TCP adapter
  //   tcpip_adapter_init();

  //   //Handle an event loop
  //   ESP_ERROR_CHECK(esp_event_loop_init(event_handler, NULL));

  //   //Initialize our wifi configuration
  //   wifi_init_config_t wifi_init_config = WIFI_INIT_CONFIG_DEFAULT();

  //   //Tell ESP to use this structure that we just declared
  //   ESP_ERROR_CHECK(esp_wifi_init(&wifi_init_config));

  //   //Tell the esp whether or not is going to be utilizing our wifi as a station mode softapp (software access point)
  //   //WIFI_MODE_AP == I can connect other things to it
  //   //WIFI_MODE_STA == You connect to a router
  //   ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));

  //   wifi_config_t wifi_config = {
  //       .sta = {
  //           .ssid = SSID,
  //           .password = PASSWORD}};

  //   //We set this configuration
  //   esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config);
  //   ESP_ERROR_CHECK(esp_wifi_start());
  // }

  // void onConnected(void *param)
  // {
  //   while (true)
  //   {
  //     //It won't proceed any further until we get an ip address
  //     if (xSemaphoreTake(onConnectionHandler, 10 * 1000 / portTICK_PERIOD_MS)) //We wait for 10 seconds
  //     {
  //       //DO SOMETHING USEFUL HERE !!!

  //       //Stop the execution of the program, wait forever so it doesn't get the information from the internet again
  //       xSemaphoreTake(onConnectionHandler, portMAX_DELAY);
  //     }

  //     else
  //     {
  //       ESP_LOGE("CONNECTION", "Could not connect");
  //       esp_restart();
  //       //the while loop will execute again
  //     }
  //   }
  // }

  // void app_main()
  // {
  //   onConnectionHandler = xSemaphoreCreateBinary();
  //   wifiInit();
  //   xTaskCreate(&onConnected, "On Connected", 1024 * 4, NULL, 5, NULL);

    /**************************************************/
    /******************* -  Wifi Scan code example *******************/
    /**************************************************/

    // #define MAX_APs 20

    //   //Method used on the esp_event_loop_init() function on the wifiInit function
    //   static esp_err_t event_handler(void *ctx, system_event_t *event)
    //   {
    //     return ESP_OK;
    //   }

    //   void wifiInit()
    //   {
    //     //Initialize Non volatile storage NVS
    //     ESP_ERROR_CHECK(nvs_flash_init());
    //     //Initialize my TCP adapter
    //     tcpip_adapter_init();

    //     //Handle an event loop
    //     ESP_ERROR_CHECK(esp_event_loop_init(event_handler, NULL));

    //     //Initialize our wifi configuration
    //     wifi_init_config_t wifi_init_config = WIFI_INIT_CONFIG_DEFAULT();

    //     //Tell ESP to use this structure that we just declared
    //     ESP_ERROR_CHECK(esp_wifi_init(&wifi_init_config));

    //     //Tell the esp whether or not is going to be utilizing our wifi as a station mode softapp (software access point)
    //     //WIFI_MODE_AP == I can connect other things to it
    //     //WIFI_MODE_STA == You connect to a router
    //     ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    //     ESP_ERROR_CHECK(esp_wifi_start());
    //   }

    //   static char *getAuthModeName(wifi_auth_mode_t auth_mode)
    //   {
    //     char *names[] = {"OPEN", "WEP", "WPA PSK", "WPA2 PSK", "WPA WPA2 PSK", "MAX"};
    //     return names[auth_mode];
    //   }

    //   void app_main()
    //   {
    //     wifiInit();
    //     //If we put 0, we are looking for everything
    //     wifi_scan_config_t wifi_scan_config = {
    //         .ssid = 0,
    //         .bssid = 0,
    //         .channel = 0,
    //         .show_hidden = true};
    //     //In order for us to use that
    //     //Block flag boolean means ... do I want to start scanning and keep the program running or do I want to leave the program running
    //     esp_wifi_scan_start(&wifi_scan_config, true);

    //     //Array of structures to retrieve results
    //     wifi_ap_record_t wifi_records[MAX_APs];

    //     //In case it doesnt find MAX_APS, it will overwrite the maxRecords variable with the actual # of APs found
    //     uint16_t maxRecords = MAX_APs;
    //     ESP_ERROR_CHECK(esp_wifi_scan_get_ap_records(&maxRecords, wifi_records));

    //     printf("Found %d access points:\n", maxRecords);
    //     printf("\n");
    //     printf("               SSID              | Channel | RSSI |   Auth Mode \n");
    //     printf("----------------------------------------------------------------\n");
    //     for (int i = 0; i < maxRecords; i++)
    //       printf("%32s | %7d | %4d | %12s\n", (char *)wifi_records[i].ssid, wifi_records[i].primary, wifi_records[i].rssi, getAuthModeName(wifi_records[i].authmode));
    //     printf("----------------------------------------------------------------\n");

    /**************************************************/
    /******************* -  Example connection: Retrieving the HTML code from www.google.com *******************/
    /**************************************************/
    // #include "protocol_examples_common.h"
    // #include "esp_wifi.h"
    // #include "esp_event.h"
    // #include "nvs_flash.h"
    // #include "esp_http_client.h"

    //   esp_err_t clientEvent(esp_http_client_event_t * evt)
    //   {
    //     switch (evt->event_id)
    //     {
    //     case HTTP_EVENT_ON_DATA:
    //       printf("%.*s\n", evt->data_len, (char *)evt->data);
    //       break;
    //     default:
    //       break;
    //     }
    //     return ESP_OK;
    //   }

    //   void app_main()
    //   {
    //     nvs_flash_init();
    //     tcpip_adapter_init();
    //     esp_event_loop_create_default();
    //     example_connect();

    //     esp_http_client_config_t clientrConfig = {
    //         .url = "http://google.com",
    //         .event_handler = clientEvent};

    //     esp_http_client_handle_t client = esp_http_client_init(&clientrConfig);
    //     esp_http_client_perform(client); //runs stuff and activates the client
    //     esp_http_client_cleanup(client);

    /**************************************************/
    /******************* -  Hibernation 5 micro Amps *******************/
    /**************************************************/
    // #define INPUT_PIN 0

    //   RTC_DATA_ATTR int timesWokenUp = 0;
    //   void app_main()
    //   {
    //     esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_OFF);
    //     esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_SLOW_MEM, ESP_PD_OPTION_ON);
    //     esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_FAST_MEM, ESP_PD_OPTION_OFF);
    //     esp_sleep_enable_timer_wakeup(5 * 1000000);
    //     printf("going to sleep. Woken up %d times\n", timesWokenUp++);

    //     esp_deep_sleep_start();

    //     printf("Waking up\n");
    //     vTaskDelay(pdMS_TO_TICKS(1000));

    /**************************************************/
    /******************* -  Deep Sleep with timer and storing variables with the RTC_DATA_ATTR *******************/
    /**************************************************/
    // #define INPUT_PIN 0

    //   RTC_DATA_ATTR int timesWokenUp = 0;
    //   void app_main()
    //   {
    //     esp_sleep_enable_timer_wakeup(5 * 1000000);
    //     printf("going to sleep. Woken up %d times\n", timesWokenUp++);

    //     esp_deep_sleep_start();

    //     printf("Waking up\n");
    //     vTaskDelay(pdMS_TO_TICKS(1000));

    /**************************************************/
    /******************* -  Light Sleep with GPIO button on PIN 0 *******************/
    /**************************************************/

    // gpio_pad_select_gpio(INPUT_PIN);
    // gpio_set_direction(INPUT_PIN, GPIO_MODE_INPUT);
    // gpio_wakeup_enable(INPUT_PIN, GPIO_INTR_LOW_LEVEL);

    // esp_sleep_enable_gpio_wakeup();
    // esp_sleep_enable_timer_wakeup(5000000);

    // while (true)
    // {
    //   if (rtc_gpio_get_level(INPUT_PIN) == 0)
    //   {
    //     printf("Please release button \n");
    //     do
    //     {

    //       vTaskDelay(pdMS_TO_TICKS(10));

    //     } while (rtc_gpio_get_level(INPUT_PIN) == 0);
    //   }
    //   printf("going for a nap\n");
    //   uart_tx_wait_idle(CONFIG_ESP_CONSOLE_UART_NUM);

    //   //for how long has the esp been running
    //   int64_t before = esp_timer_get_time();

    //   esp_light_sleep_start();
    //   int64_t after = esp_timer_get_time();

    //   //reason of waking up?
    //   esp_sleep_wakeup_cause_t reason = esp_sleep_get_wakeup_cause();

    //   printf("Before =  %lld miliseconds\nAfter =  %lld miliseconds\nNaptime =  %lld miliseconds\nReason was %s\n", before / 1000, after / 1000, (after - before) / 1000, reason == ESP_SLEEP_WAKEUP_TIMER ? "Timer" : "Button");
    // }

    /**************************************************/
    /******************* -  Light Sleep with Timer *******************/
    /**************************************************/
    // void app_main()
    // {
    //   esp_sleep_enable_timer_wakeup(5000000);
    //   printf("going for a nap\n");
    //   uart_tx_wait_idle(CONFIG_ESP_CONSOLE_UART_NUM);

    //   //for how long has the esp been running
    //   int64_t before = esp_timer_get_time();

    //   esp_light_sleep_start();

    //   int64_t after = esp_timer_get_time();
    //   printf("Went for a nap for %lld miliseconds\n", (after - before) / 1000);

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

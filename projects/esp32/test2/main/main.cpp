#include <stdio.h>

#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <string>
#include <iostream>
#include "esp_log.h"

#define PIN GPIO_NUM_2
using namespace std;
#define TAG "CONFIG"

// see https://www.learnesp32.com/3_blinkey for details
void blinky(void *params)
{
	gpio_pad_select_gpio(PIN);
	gpio_set_direction(PIN, GPIO_MODE_OUTPUT);
	int isOn = 0;
	while (true)
	{
		isOn = !isOn;
		gpio_set_level(PIN, isOn);
		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
}
class Person
{
  int _age;
  string _name;

public:
  Person(string, int);
  void SayHello()
  {
    cout << "Hi I'm " << this->_name << " and I'm " << this->_age << " years old\n";
  }
};

Person::Person(string name, int age)
{
  _name = name;
  _age = age;
}
// see https://www.learnesp32.com/7_menuconfig-introduction for details
void config(void *params)
{
    ESP_LOGI(TAG, "MY_INT %d", CONFIG_MY_INT);
    ESP_LOGI(TAG, "MY_STRING %s", CONFIG_MY_STRING);
  
    bool my_bool = false;
  #ifdef CONFIG_MY_BOOL
    my_bool = true;
  #else
    my_bool = false;
  #endif
  
    ESP_LOGI(TAG, "MY_BOOL %s", my_bool ? "yes" : "no");
  
    int option = 0;
  
  #ifdef CONFIG_OPTION_1
        option = 1;
  #elif CONFIG_OPTION_2
        option = 2;
  #else
        option = 3;
  #endif
  
   ESP_LOGI(TAG, "MY_OPTION %d", option);
   while(true)
      vTaskDelay(1000 / portTICK_PERIOD_MS);
}
extern "C" void app_main(void)
{
  printf("Hello world!\n");
  
  xTaskCreate(&blinky, "blink led", 2048, NULL, 2, NULL);
   Person john("John", 23);
   john.SayHello();
  xTaskCreate(&config, "config demo", 2048, NULL, 2, NULL);
}

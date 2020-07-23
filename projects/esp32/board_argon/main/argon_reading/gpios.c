
#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG_GPIOs = "GPIOs";

#define PIN_FAN 22 // Pins 34-39 are input only !!!!
#define PIN_LED_1 12 // Pins 34-39 are input only !!!!
#define PIN_LED_2 21 // Pins 34-39 are input only !!!!

void activateGPIOs(void *params)
{   
    printf("---------- Starting FAN and LEDs----------\n");
    gpio_pad_select_gpio(PIN_FAN);
    gpio_set_direction(PIN_FAN, GPIO_MODE_OUTPUT);
    gpio_pad_select_gpio(PIN_LED_1);
    gpio_set_direction(PIN_LED_1, GPIO_MODE_OUTPUT);
    gpio_pad_select_gpio(PIN_LED_2);
    gpio_set_direction(PIN_LED_2, GPIO_MODE_OUTPUT);
    int isOn = 0;
    while (true)
    {
        isOn = !isOn;
        printf("---------- LED FFLASHING ----------\n");

        gpio_set_level(PIN_FAN, isOn);
        gpio_set_level(PIN_LED_1, isOn);
        gpio_set_level(PIN_LED_2, isOn);
        vTaskDelay(3000 / portTICK_PERIOD_MS);
    }
}

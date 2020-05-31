#include <driver/gpio.h>
#include <driver/spi_master.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <stdio.h>
#include <string.h>
//#include <u8g2.h>


#include <Wire.h> //Needed for I2C to GPS
#include "SparkFun_Ublox_Arduino_Library.h" //Click here to get the library: http://librarymanager/All#SparkFun_Ublox_GPS

#define PIN_CLK 14
#define PIN_MOSI 13
#define PIN_RESET 26
#define PIN_DC 27
#define PIN_CS 15



SFE_UBLOX_GPS myGPS;


extern "C"
{
    void app_main();
}

void app_main(void)

{ 	Serial.begin(115200);
  	Serial.println("SparkFun Ublox Example");
    Wire.begin();

    delay(250); //Don't pound too hard on the I2C bus

}

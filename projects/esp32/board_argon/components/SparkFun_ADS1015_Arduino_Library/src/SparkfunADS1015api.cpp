/*
  Using the SparkFun Qwiic 12 Bit ADC - 4 Channel ADS1015
  By: Pete Lewis, Original flex-glove library by: Andy England
  SparkFun Electronics
  Date: May 9, 2019
  License: This code is public domain but you can buy me a beer if you use this and we meet someday (Beerware license).

  Feel like supporting our work? Please buy a board from SparkFun!
  https://www.sparkfun.com/products/15334

  This example shows how to output ADC values on one single-ended channel (A3).
  *at default gain setting of 1 (and 3.3V VCC), 0-2V will read 0-2047.
  *anything greater than 2V will read 2047.

  Hardware Connections and initial setup:
  Plug in your controller board (e.g. Redboard Qwiic) into your computer with USB cable.
  Connect your Qwiic 12 Bit ADC board to your controller board via a qwiic cable.
  Select TOOLS>>BOARD>>"Arduino/Genuino Uno"
  Select TOOLS>>PORT>> "COM 3" (note, yours may be different)
  Click upload, and watch streaming data over serial monitor at 9600.

*/
#include <SparkFun_ADS1015_Arduino_Library.h> //Click here to get the library: http://librarymanager/All#SparkFun_ADS1015
#include <Wire.h>

// ADS1015_CONFIG_PGA_TWOTHIRDS  +/- 6.144v
// ADS1015_CONFIG_PGA_1          +/- 4.096v (used in this example)
// ADS1015_CONFIG_PGA_2          +/- 2.048v
// ADS1015_CONFIG_PGA_4          +/- 1.024v
// ADS1015_CONFIG_PGA_8          +/- 0.512v
// ADS1015_CONFIG_PGA_16         +/- 0.256v

ADS1015 adcSensor;

extern "C" void startADC()
{
  printf("SparkfunADS1015api.cpp: Wire begin .... \n");
  Wire1.begin(33, 32); // begin(sda,scl) --> SDA Blue 16/26, SCL Yellow 17/25
  //Wire.begin(26, 25); // begin(sda,scl) --> SDA Blue 16/26, SCL Yellow 17/25
  if (adcSensor.begin(0x48, Wire1) == true)
  {
    printf("SparkfunADS1015api.cpp: startADC(): Device found. I2C connections are good.\n");
  }
  else
  {
    printf("Device not found. Check wiring.\n");
  }
  adcSensor.setGain(ADS1015_CONFIG_PGA_1); // PGA gain set to 1
}

extern "C" void ADS1X15_get_raw(int *differential_a0a3, int *differential_a1a3, int *differential_a2a3)
{
  // int16_t adc0, adc1, adc2, adc3;
  // adc0 = adcSensor.getSingleEnded(0);
  // adc1 = adcSensor.getSingleEnded(1);
  // adc2 = adcSensor.getSingleEnded(2);
  // adc3 = adcSensor.getSingleEnded(3);
  //  printf("AIN0: %d\t", adc0);
  // printf("AIN1: %d\t", adc1);
  // printf("AIN2: %d\t", adc2);
  // printf("AIN3: %d\t", adc3);

  // int16_t differential = adcSensor.getDifferential(); // default (i.e. no arguments) is A0 and A1
  // Optional "commented out" examples below show how to read differential readings between other pins
  // int16_t differential = adcSensor.getDifferential(ADS1015_CONFIG_MUX_DIFF_P0_N3); //Between A0 and A3
  //int16_t differential = adcSensor.getDifferential(ADS1015_CONFIG_MUX_DIFF_P1_N3);
  //int16_t differential = adcSensor.getDifferential(ADS1015_CONFIG_MUX_DIFF_P2_N3);

  *differential_a2a3 = adcSensor.getDifferential(ADS1015_CONFIG_MUX_DIFF_P0_N3); //Between A2 and A3
  *differential_a0a3 = adcSensor.getDifferential(ADS1015_CONFIG_MUX_DIFF_P1_N3); //Between A0 and A3
  *differential_a1a3 = adcSensor.getDifferential(ADS1015_CONFIG_MUX_DIFF_P2_N3); //Between A1 and A3
  

  //Just used for debugging
  // printf("DIFFERENTIAL A0 A3: %d\t", *differential_a0a3); 
  // printf("DIFFERENTIAL A1 A3: %d\t", *differential_a1a3);
  // printf("DIFFERENTIAL A2 A3: %d\t", *differential_a2a3);
  // printf("\n");
}

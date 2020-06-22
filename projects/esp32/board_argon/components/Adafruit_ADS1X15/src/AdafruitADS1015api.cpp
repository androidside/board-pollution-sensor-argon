
#include <Adafruit_ADS1015.h> //Click here to get the library: http://librarymanager/All#SparkFun_ADS1015
#include <Wire.h>

 Adafruit_ADS1115 ads;  /* Use this for the 16-bit version */
//Adafruit_ADS1015 ads; /* Use thi for the 12-bit version */

// The ADC input range (or gain) can be changed via the following
// functions, but be careful never to exceed VDD +0.3V max, or to
// exceed the upper and lower limits if you adjust the input range!
// Setting these values incorrectly may destroy your ADC!
//                                                                ADS1015  ADS1115
//                                                                -------  -------
// ads.setGain(GAIN_TWOTHIRDS);  // 2/3x gain +/- 6.144V  1 bit = 3mV      0.1875mV (default)
// ads.setGain(GAIN_ONE);        // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV
// ads.setGain(GAIN_TWO);        // 2x gain   +/- 2.048V  1 bit = 1mV      0.0625mV
// ads.setGain(GAIN_FOUR);       // 4x gain   +/- 1.024V  1 bit = 0.5mV    0.03125mV
// ads.setGain(GAIN_EIGHT);      // 8x gain   +/- 0.512V  1 bit = 0.25mV   0.015625mV
// ads.setGain(GAIN_SIXTEEN);    // 16x gain  +/- 0.256V  1 bit = 0.125mV  0.0078125mV

extern "C" void startADC_adafruit()
{  
  
  //ads.begin();
  //Wire1.begin();
//****************************************************************************************************//
// Important Note; I had to change the Adafruit_ADS1015.cpp library to use Wire1() instead of Wire() //
//****************************************************************************************************//
  printf("AdafruitADS1015api.cpp: Wire begin .... \n");
  Wire1.begin(33, 32); // begin(sda,scl) --> SDA Blue 16/26, SCL Yellow 17/25

  ads.setGain(GAIN_ONE); // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV // PGA gain set to 1
}

extern "C" void ADS1X15_get_raw_adafruit(int *differential_a0a3, int *differential_a1a3, int *differential_a2a3)
{
  int16_t adc0=0, adc1=0, adc2=0, adc3=0;

  adc0 = ads.readADC_SingleEnded(0);
  adc1 = ads.readADC_SingleEnded(1);
  adc2 = ads.readADC_SingleEnded(2);
  adc3 = ads.readADC_SingleEnded(3);
  printf("AIN0: %d\t", adc0);
  printf("AIN1: %d\t", adc1);
  printf("AIN2: %d\t", adc2);
  printf("AIN3: %d\t", adc3);

  int16_t results_0_1 = 0 ;
  int16_t results_2_3 = 0 ;

  /* Be sure to update this value based on the IC and the gain settings! */
  float multiplier = 3.0F; /* ADS1015 @ +/- 6.144V gain (12-bit results) */
  //float multiplier = 0.1875F; /* ADS1115  @ +/- 6.144V gain (16-bit results) */

  results_0_1 = ads.readADC_Differential_0_1();

  results_2_3 = ads.readADC_Differential_2_3();

  printf("Differential_0_1: %d\t Differential_2_3: %d\t", results_0_1, results_2_3);

  //Just used for debugging
  // printf("DIFFERENTIAL A0 A3: %d\t", *differential_a0a3);
  // printf("DIFFERENTIAL A1 A3: %d\t", *differential_a1a3);
  // printf("DIFFERENTIAL A2 A3: %d\t", *differential_a2a3);
  printf("\n");
}

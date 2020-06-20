#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/adc.h"
#include "esp_log.h"
#include <math.h>
#include "esp_adc_cal.h"
#include "AdafruitADS1015api.h"
#include "freertos/semphr.h"

#define TAG_NO2 "NO2ADAFRUIT.C"
#define SENSITIVITY_CODE_NO_SENSOR 27.64
#define TIA 100 // kV/A (RGain of Trans impedance amplifier circuit is 100,000 ohms), the TIA it is Rgain in Kilo ohm units, we'll multiply by 1000 to have ohms

float vgas_adafruit;
float vref_adafruit;
float vgas0_adafruit = 1;
float ppm_adafruit;

int rgain_adafruit = 100000;
extern xSemaphoreHandle mutexBus;


void activateNO2adafruit(void *params)
{

    //Setup ADC reading wire()

    printf("---------- NO2.C: Starting ADC ----------\n");
    if (xSemaphoreTake(mutexBus, 10000 / portTICK_PERIOD_MS))
    {
        startADC_adafruit();
        xSemaphoreGive(mutexBus);
    }
    printf("---------- NO2.C: ADC Started ----------\n");

    //     static const adc_channel_t channel = ADC_CHANNEL_6;     //
    //     static const adc_bits_width_t width = ADC_WIDTH_BIT_12;
    //     static const adc_atten_t atten = ADC_ATTEN_DB_6;
    //     static const adc_unit_t unit = ADC_UNIT_1;
    //    //Configure analog in voltage read pins
    //    adc1_config_width(width); //12 bits, 4095 values
    //    adc1_config_channel_atten(channel, atten); //ADC1_CH4, GPIO32 pin 12, the input voltage of ADC will be reduced to about 1/3.6

    //     // Calibration curve
    //     //Characterize ADC at particular atten
    //     esp_adc_cal_characteristics_t *adc_chars = calloc(1, sizeof(esp_adc_cal_characteristics_t));
    //     esp_adc_cal_value_t val_type = esp_adc_cal_characterize(unit, atten, width, 1107, adc_chars);
    //     //Check type of calibration value used to characterize ADC
    //     if (val_type == ESP_ADC_CAL_VAL_EFUSE_VREF) {
    //         printf("eFuse Vref\n");
    //     } else if (val_type == ESP_ADC_CAL_VAL_EFUSE_TP) {
    //         printf("Two Point\n");
    //     } else {
    //         printf("Default\n");
    //     }

    while (true)
    {
        if (xSemaphoreTake(mutexBus, 1000 / portTICK_PERIOD_MS))
        {

            int readings[3] = {0, 0, 0};
            int averageSamples = 1;
            int adcReadingAveraged[3] = {0, 0, 0};
            bool isneg[3] = {false, false, false};
            int differential_a0a3 = 0, differential_a1a3 = 0, differential_a2a3 = 0;
            //read thre values and add them up
            for (int i = 0; i < averageSamples; i++)
            {
                ADS1X15_get_raw_adafruit(&differential_a0a3, &differential_a1a3, &differential_a2a3); //ADS1X15_get_raw();;
                readings[0] += differential_a0a3;
                readings[1] += differential_a1a3;
                readings[2] += differential_a2a3;
            }
            xSemaphoreGive(mutexBus);
            //for each of the 3 values calculate the average and sign
            for (int i = 0; i < 3; i++)
            {

                if (readings[i] < 0)
                {
                    isneg[i] = true;
                    //printf("isneg=true\n");
                }

                else
                {
                    isneg[i] = false;
                    //printf("isneg=false\n");
                }
                readings[i] = abs(readings[i]);

                adcReadingAveraged[i] = round((float)readings[i] / averageSamples); //We make one of the 2 numbers of the division a float so the division renders a float as a result and we round it properly

                if (isneg[i])
                {
                    adcReadingAveraged[i] = -adcReadingAveraged[i];
                    //printf("changing sign...adcReadingAveraged = -adcReadingAveraged\n");
                }
                //uint32_t voltage = esp_adc_cal_raw_to_voltage(adcReadingAveraged, adc_chars);
            }
            //Convert adc readings to voltage value (based on gain)
            float voltage[3] = {0, 0, 0};
            voltage[0] = adcReadingAveraged[0] * 0.002;
            voltage[1] = adcReadingAveraged[1] * 0.002;
            voltage[2] = adcReadingAveraged[2] * 0.002;

 // ESP_LOGI(TAG_NO2, "reading averaged[0] = %d\treading averaged[1] = %d\treading averaged[2] = %d\n ", adcReadingAveraged[0], adcReadingAveraged[1], adcReadingAveraged[2]);
            //vgas =  ((float)reading / 4095)*1.107*2;
            //printf("vgas is %f volts\n", vgas);
//            printf("Voltage A0-A3=%f V\tVoltage A1-A3=%f V\tVoltage A2-A3=%f V\n", voltage[0], voltage[1], voltage[2]);
            vgas_adafruit = voltage[0]; //Between A0 and A3
            vref_adafruit = voltage[1]; // Between A1 and A3
            //perform operations
            float M = SENSITIVITY_CODE_NO_SENSOR * TIA * 1000 * 0.000000001;
            ppm_adafruit = (vgas_adafruit - vgas0_adafruit) / M;

            //populate vgas, ppm

            //sleep
            vTaskDelay(2000 / portTICK_PERIOD_MS);
        }

        else
        {
            ESP_LOGE(TAG_NO2, "no2.c: mutexBus timeout");
        }
    }
}

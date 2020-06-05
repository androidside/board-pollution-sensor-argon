#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/adc.h"
#include "esp_log.h"
#include <math.h>
#include "esp_adc_cal.h"
#include "SparkfunADS1015api.h"


#define TAG_NO2 "NO2.C"

float vgas;
float vgas0 = 1;
float ppm;
int adcReadingAveraged;
int rgain = 100000;

float readvgas()
{
    ESP_LOGI(TAG_NO2, "**** readvgas() : vgas = %f ****", vgas);
    return vgas;  
}

float read_adc_reading_averaged()
{
    return adcReadingAveraged;
}

float readvgas0()
{
    return vgas0;
    ESP_LOGI(TAG_NO2, "vgas0 = %f", vgas0);
}

float readppm()
{
    return ppm;
}

int readrgain()
{
    return rgain;
}

void activateNO2(void *params)
{


//Setup ADC reading wire()

printf("---------- NO2.C: Starting ADC ----------\n");
startADC();
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


    while(true){
         
        int reading = 0;
        int averageSamples = 10 ;
        bool isneg = false;
        //read value
        for (int i = 0; i < averageSamples; i++)
        {
            reading += ADS1X15_get_raw();//ADS1X15_get_raw();;
        }
        if(reading < 0){
            isneg=true;
            //printf("isneg=true\n");
        }

        else{
            isneg=false;
            //printf("isneg=false\n");
        }
        reading = abs(reading);
        
        adcReadingAveraged = round((float)reading/averageSamples); //We make one of the 2 numbers of the division a float so the division renders a float as a result and we round it properly
        
        if(isneg){
            adcReadingAveraged = -adcReadingAveraged;
            //printf("changing sign...adcReadingAveraged = -adcReadingAveraged\n");
        }
        //uint32_t voltage = esp_adc_cal_raw_to_voltage(adcReadingAveraged, adc_chars);

        float voltage = adcReadingAveraged *0.002;
        ESP_LOGI(TAG_NO2, "reading averaged = %d", adcReadingAveraged);
        //vgas =  ((float)reading / 4095)*1.107*2;
        //printf("vgas is %f volts\n", vgas);
        printf("voltage calibrated is %f volts\n", voltage);
        vgas = voltage;
        //perform operations
        //populate vgas, ppm

        //sleep
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
#include "Arduino.h"
#include <Wire.h> //Needed for I2C to GPS
#include <string>
#include "esp_log.h"

#include "SparkFun_Ublox_Arduino_Library.h" //http://librarymanager/All#SparkFun_Ublox_GPS


#define TAG_GPS "GPS.cpp"
// SFE_UBLOX_GPS myGPS;

// long lastTime = 0; //Simple local timer. Limits amount if I2C traffic to Ublox module.
// using std::string; //that is so we dont have to write std::string when declaring a string
// string latitudestring("");
// string longitudestring("");
// string datetimestring("");
// char *latitude_char_array;

// const char *readlatitude()
// {
//     return latitude_char_array;
// }



void activateGPS(void *params)
{
    ESP_LOGE(TAG_GPS, "Ublox GPS not detected at default I2C address. Please check wiring.");
    // //initArduino();
    // printf("Activating GPS ... \n");

    // Wire.begin(15, 4); // begin(sda,scl) --> SDA Blue cable, SCL Yellow cable

    // if (myGPS.begin() == false) //Connect to the Ublox module using Wire port
    // {
    //     ESP_LOGE(TAG_GPS, "Ublox GPS not detected at default I2C address. Please check wiring.");
    //     vTaskDelay(1000 / portTICK_PERIOD_MS);
    // }

    // myGPS.setI2COutput(COM_TYPE_UBX); //Set the I2C port to output UBX only (turn off NMEA noise)
    // myGPS.saveConfiguration();        //Save the current settings to flash and BBR

    // while (true)
    // {
    //     //Query module only every second. Doing it more often will just cause I2C traffic.
    //     //The module only responds when a new position is available

    //     using std::string; //that is so we dont have to write std::string when declaring a string
    //     latitudestring = std::to_string((float)myGPS.getLatitude() / 10000000);
    //     longitudestring = std::to_string((float)myGPS.getLongitude() / 10000000);
    //     int year_int = myGPS.getYear(); //will be used to determine if the data is valid
    //     string year = std::to_string(year_int);
    //     string month = std::to_string(myGPS.getMonth());
    //     string day = std::to_string(myGPS.getDay());
    //     string hour = std::to_string(myGPS.getHour());
    //     string minute = std::to_string(myGPS.getMinute());
    //     string second = std::to_string(myGPS.getSecond());

    //     datetimestring.append(year).append("-").append(month).append("-").append(day).append(" ").append(hour).append(":").append(minute).append(":").append(second).append(".0");

    //     if (year_int == 0) //If GPS information not avaialble, we will deal with it on sever
    //     {
    //         datetimestring.assign("-1");
    //         latitudestring.assign("0");
    //         longitudestring.assign("0");
    //     }
    //     ESP_LOGI(TAG_GPS, "\nLAT_string: %s, LON_string: %s, datetime_string: %s", latitudestring.c_str(), longitudestring.c_str(), datetimestring.c_str()); //Convert String object to C string (char*)
    //     strcpy(latitude_char_array, latitudestring.c_str());
    //     vTaskDelay(1000 / portTICK_PERIOD_MS);
    //}
}

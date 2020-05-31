//file: main.cpp
#include "Arduino.h"
#include <Wire.h> //Needed for I2C to GPS

#include "SparkFun_Ublox_Arduino_Library.h" //http://librarymanager/All#SparkFun_Ublox_GPS
//SFE_UBLOX_GPS myGPS;

#define TAG "Example"
SFE_UBLOX_GPS myGPS;
long lastTime = 0; //Simple local timer. Limits amount if I2C traffic to Ublox module.

extern "C" void app_main()
{
    initArduino();
    printf("Hello World This i arduino talking\n");
    pinMode(4, OUTPUT);
    digitalWrite(4, HIGH);
    //do your own thing

    while (!Serial)
        ; //Wait for user to open terminal
    printf("SparkFun Ublox Example");

    Wire.begin(15,2);

    if (myGPS.begin() == false) //Connect to the Ublox module using Wire port
    {
        printf("Ublox GPS not detected at default I2C address. Please check wiring. Freezing.");
        
    }

    myGPS.setI2COutput(COM_TYPE_UBX); //Set the I2C port to output UBX only (turn off NMEA noise)
    myGPS.saveConfiguration();        //Save the current settings to flash and BBR

    while (true)
    {
        //Query module only every second. Doing it more often will just cause I2C traffic.
        //The module only responds when a new position is available
        if (millis() - lastTime > 2000)
        {
            lastTime = millis(); //Update the timer

            long latitude = myGPS.getLatitude();
           printf("Lat: %ld", latitude);

            long longitude = myGPS.getLongitude();
           printf(" Long:  %ld", longitude);
           printf(" (degrees * 10^-7)");

            long altitude = myGPS.getAltitude();
           printf(" Alt:  %ld", altitude);
           printf(" (mm)");

           printf("\n");
        }
    }
}

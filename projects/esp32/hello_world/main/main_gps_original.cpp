// //file: main.cpp
// #include "Arduino.h"
// #include <Wire.h> //Needed for I2C to GPS
// #include <string>

// #include "SparkFun_Ublox_Arduino_Library.h" //http://librarymanager/All#SparkFun_Ublox_GPS
// #include "Adafruit_ADS1015.h"

// #define TAG "Example"
// SFE_UBLOX_GPS myGPS;
// Adafruit_ADS1015 ads; /* Use thi for the 12-bit version */

// long lastTime = 0; //Simple local timer. Limits amount if I2C traffic to Ublox module.

// extern "C" void app_main()
// {
//     //initArduino();
//     printf("Hello World no initArduino() from the HELLO WORLD PROGRAM! \n");

//     //do your own thing

//     printf("SparkFun Ublox Example");

//     Wire.begin(15, 4); // begin(sda,scl) --> SDA Blue cable, SCL Yellow cable

//     if (myGPS.begin() == false) //Connect to the Ublox module using Wire port
//     {
//         printf("Ublox GPS not detected at default I2C address. Please check wiring. Freezing.");
//     }

//     myGPS.setI2COutput(COM_TYPE_UBX); //Set the I2C port to output UBX only (turn off NMEA noise)
//     myGPS.saveConfiguration();        //Save the current settings to flash and BBR

//     while (true)
//     {
//         //Query module only every second. Doing it more often will just cause I2C traffic.
//         //The module only responds when a new position is available
//         if (millis() - lastTime > 2000)
//         {
//             lastTime = millis(); //Update the timer

//             long latitude = myGPS.getLatitude();
//             printf("Lat: %ld", latitude);

//             long longitude = myGPS.getLongitude();
//             printf(" Long:  %ld", longitude);
//             printf(" (degrees * 10^-7)");

//             long altitude = myGPS.getAltitude();
//             printf(" Alt:  %ld", altitude);
//             printf(" (mm)");

//             byte SIV = myGPS.getSIV();
//             printf(" SIV: %hhd", SIV);

//             printf("%hu", myGPS.getYear());
//             printf("-");
//             printf("%hhu", myGPS.getMonth());
//             printf("-");
//             printf("%hhu", myGPS.getDay());
//             printf(" ");
//             printf("%hhu", myGPS.getHour());
//             printf(":");
//             printf("%hhu", myGPS.getMinute());
//             printf(":");
//             printf("%hhu", myGPS.getSecond());

//             using std::string; //that is so we dont have to write std::string when declaring a string
//             string latitudestring = std::to_string((float) myGPS.getLatitude() / 10000000);
//             string longitudestring = std::to_string((float) myGPS.getLongitude() / 10000000);

//             string datetimestring("");

//             int year_int = myGPS.getYear(); //will be used to determine if the data is valid
//             string year = std::to_string(year_int);
//             string month = std::to_string(myGPS.getMonth());
//             string day = std::to_string(myGPS.getDay());
//             string hour = std::to_string(myGPS.getHour());
//             string minute = std::to_string(myGPS.getMinute());
//             string second = std::to_string(myGPS.getSecond());

//             datetimestring.append(year).append("-").append(month).append("-").append(day).append(" ").append(hour).append(":").append(minute).append(":").append(second).append(".0");

//             if (year_int == 0) //If GPS information not avaialble, we will deal with it on sever
//             {
//                 datetimestring.assign("-1");
//             }
//             printf("\nlatitudestring: %s ", latitudestring.c_str());
//             printf("longitudestring: %s ", longitudestring.c_str());
//             printf("datetimestring: %s\n", datetimestring.c_str()); //Convert String object to C string (char*)
//         }
//     }
// }

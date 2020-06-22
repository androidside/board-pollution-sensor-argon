#include "Arduino.h"
#include <Wire.h> //Needed for I2C to GPS
#include <string>
#include "esp_log.h"

#include "SparkFun_Ublox_Arduino_Library.h" //http://librarymanager/All#SparkFun_Ublox_GPS

#define TAG_GPS "GPS.cpp"
SFE_UBLOX_GPS myGPS;

long lastTime = 0; //Simple local timer. Limits amount if I2C traffic to Ublox module.
using std::string; //that is so we dont have to write std::string when declaring a string
float latitude;
float longitude;
string datetimestring("");
char *latitude_char_array;

// const char *readlatitude()
// {
//     return latitude_char_array;
// }

// const char *readlongitude()
// {
//     char *ch = new char[longitudestring.length() + 1] /*  len of string + 1 char for \0*/;
//     strcpy(ch, longitudestring.c_str());
//     return ch;
// }

// const char *readdatetime()
// {
//     char *ch = new char[datetimestring.length() + 1] /*  len of string + 1 char for \0*/;
//     strcpy(ch, datetimestring.c_str());
//     return ch;
// }

extern "C" void startGPS()
{

    //initArduino();
    printf("Activating GPS ... \n");

    Wire.begin(26, 25); // begin(sda,scl) --> SDA Blue 26, SCL Yellow 25

    if (myGPS.begin() == false) //Connect to the Ublox module using Wire port
    {
        printf("GPS.cpp: Ublox GPS not detected at default I2C address. Please check wiring.\n");
    }

    myGPS.setI2COutput(COM_TYPE_UBX); //Set the I2C port to output UBX only (turn off NMEA noise)
    myGPS.saveConfiguration();        //Save the current settings to flash and BBR
}
extern "C" void readGPS(float *latitude_param, float *longitude_param, char *datetime_param)
{
    //Query module only every second. Doing it more often will just cause I2C traffic.
    //The module only responds when a new position is available

    using std::string; //that is so we dont have to write std::string when declaring a string
    latitude = (float)myGPS.getLatitude() / 10000000;
    longitude = (float)myGPS.getLongitude() / 10000000;
    int year_int = myGPS.getYear(); //will be used to determine if the data is valid
    string year = std::to_string(year_int);
    string month = std::to_string(myGPS.getMonth());
    string day = std::to_string(myGPS.getDay());
    string hour = std::to_string(myGPS.getHour());
    string minute = std::to_string(myGPS.getMinute());
    string second = std::to_string(myGPS.getSecond());

    datetimestring.append(year).append("-").append(month).append("-").append(day).append(" ").append(hour).append(":").append(minute).append(":").append(second).append(".0");

    if (year_int == 0) //If GPS information not avaialble, we will deal with it on sever
    {
        datetimestring.assign("-1");
        latitude = 0;
        longitude = 0;
    }

    // We reset the string on C so we don't keep appending datetimestring strings to it
    memset(datetime_param, 0, 128);

    //Assign values to addresses to be retrieved by the C code
    *longitude_param = longitude; //1) longitude_param is an address 2) if we write *longitude_param we dereference the pointer  3) so *longiude_param = longitude --> content located at the particular address of *latitude_param is asigned to longitude
    *latitude_param = latitude;
    strcpy(datetime_param, datetimestring.c_str());

    printf("\n GPS.cpp: LAT: %f, LON: %f, datetime_string: %s\n", latitude, longitude, datetimestring.c_str()); //Convert String object to C string (char*)
    datetimestring.clear();
}

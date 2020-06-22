/******************************************************************************
TMP102_example.ino
Example for the TMP102 I2C Temperature Sensor
Alex Wende @ SparkFun Electronics
April 29th 2016
~

This sketch configures the TMP102 temperature sensor and prints the
temperature and alert state (both from the physical pin, as well as by
reading from the configuration register.

Resources:
Wire.h (included with Arduino IDE)
SparkFunTMP102.h

Development environment specifics:
Arduino 1.0+

This code is beerware; if you see me (or any other SparkFun employee) at
the local, and you've found our code helpful, please buy us a round!

Distributed as-is; no warranty is given.   
******************************************************************************/

// Include the SparkFun TMP102 library.
// Click here to get the library: http://librarymanager/All#SparkFun_TMP102

#include <Wire.h>           // Used to establied serial communication on the I2C bus
#include <SparkFunTMP102.h> // Used to send and recieve specific information from our sensor

// Connections
// VCC = 3.3V
// GND = GND
// SDA = A4
// SCL = A5
//const int ALERT_PIN = A3;

TMP102 sensor0;

extern "C" void startTMP102()
{
    printf("Activating TMP102 ... \n");

    /************************************************************************************************/
    /****************** IMPORTANT !!!! Wire.begin(26,25) started on gpsapi.cpp *********************/
    /**********************************************************************************************/

    //Wire.begin(26, 25); // begin(sda,scl) --> SDA Blue 26, SCL Yellow 25

    //pinMode(ALERT_PIN, INPUT); // Declare alertPin as an input

    /* The TMP102 uses the default settings with the address 0x48 using Wire.
  
     Optionally, if the address jumpers are modified, or using a different I2C bus,
	 these parameters can be changed here. E.g. sensor0.begin(0x49,Wire1)
	 
	 It will return true on success or false on failure to communicate. */
    if (!sensor0.begin())
    {
        printf("Cannot connect to TMP102.\n");
        printf("Is the board connected? Is the device ID correct?\n");
        // while (1)
        //     ;
    }

    printf("Connected to TMP102!\n");
    delay(100);

    // Initialize sensor0 settings
    // set the number of consecutive faults before triggering alarm.
    // 0-3: 0:1 fault, 1:2 faults, 2:4 faults, 3:6 faults.
    sensor0.setFault(0); // Trigger alarm immediately

    // set the polarity of the Alarm. (0:Active LOW, 1:Active HIGH).
    sensor0.setAlertPolarity(1); // Active HIGH

    // set the sensor in Comparator Mode (0) or Interrupt Mode (1).
    sensor0.setAlertMode(0); // Comparator Mode.

    // set the Conversion Rate (how quickly the sensor gets a new reading)
    //0-3: 0:0.25Hz, 1:1Hz, 2:4Hz, 3:8Hz
    sensor0.setConversionRate(2);

    //set Extended Mode.
    //0:12-bit Temperature(-55C to +128C) 1:13-bit Temperature(-55C to +150C)
    sensor0.setExtendedMode(0);

    //set T_HIGH, the upper limit to trigger the alert on
    sensor0.setHighTempF(85.0); // set T_HIGH in F
    //sensor0.setHighTempC(29.4); // set T_HIGH in C

    //set T_LOW, the lower limit to shut turn off the alert
    sensor0.setLowTempF(84.0); // set T_LOW in F
                               //sensor0.setLowTempC(26.67); // set T_LOW in C
}

extern "C" float readTemperatureTMP102api()
{
    float temperature;
    //boolean alertPinState, alertRegisterState;

    // Turn sensor on to start temperature measurement.
    // Current consumtion typically ~10uA.
    sensor0.wakeup();

    // read temperature data
    temperature = sensor0.readTempC();
    //temperature = sensor0.readTempC();

    // Check for Alert
    //alertPinState = digitalRead(ALERT_PIN); // read the Alert from pin
    //alertRegisterState = sensor0.alert();   // read the Alert from register

    // Place sensor in sleep mode to save power.
    // Current consumtion typically <0.5uA.
    sensor0.sleep();

    // Print temperature and alarm state
    printf("SparkFunTMP102api.cpp: Temperature TMP102: %f\n", temperature);

    return temperature;

    //   Serial.print("\tAlert Pin: ");
    //   Serial.print(alertPinState);

    //   Serial.print("\tAlert Register: ");
    //   Serial.println(alertRegisterState);
}
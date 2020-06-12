#ifndef _GPS_H
#define _GPS_H

// const char* readlatitude_api();
// const char* readlongitude_api();
// const char* readdatetime_api();

float getlatitude();
float getlongitude();
const char *getdatetime();
void activateGPS(void *params); //We add the void activateGPS(void *params) to gps.h so we can call it from main() task

#endif
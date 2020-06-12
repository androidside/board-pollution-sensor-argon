#ifndef _NO2_H
#define _NO2_H

float readvgas();
float readvref();
float readvgas0();
float readppm();
int readrgain();


void activateNO2(void *params); //We add the void activateNO2(void *params) to no2.h so we can call it from main() task

#endif
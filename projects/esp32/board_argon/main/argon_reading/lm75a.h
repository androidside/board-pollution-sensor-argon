#ifndef _LM75A_H
#define _LM75A_H

float readTemperatureLM75A();
void activateLM75A(void *params); //We add the void activateLM75A(void *params) to LM75A.h so we can call it from main() task

#endif
#ifndef _FETCH_H_
#define _FETCH_H_

#include "argon_reading/reading.h"

typedef struct{

    char *key;
    char *val;
}header_t;

typedef enum{
    GET,
    POST
}httpMethod;

//Function pointer
struct fetchParams_t{
    void (*OnGotData) (char *incomingBuffer, char *output); //OnGotData is only used to parse Data on HTTP Get requests
    char message[300]; // Message received from the HTTP GET request, 300 byte array, we could do malloc later on and allow for larger messages but for now we keep it at 300 to be on the safe side.
    header_t header[3];
    int headerCount; //sometimes we will have headers and sometimes we won't, its good to have a count to know how many headers we have
    httpMethod method;
    char *body; //Body to send on the HTTP Post Request
    int status; //Code returned on the http method
};

void fetch(char* url, struct fetchParams_t *fetchParams);

int postReading(struct reading_t *reading);
void createBodyReading(struct reading_t *reading, char *body);

#endif
#ifndef _FETCH_H_
#define _FETCH_H_

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
    void (*OnGotData) (char *incomingBuffer, char *output);
    char message[300]; //300 byte array, we could do malloc and things like that
    header_t header[3];
    int headerCount; //sometimes we will have headers and sometimes we won't, its good to have a count to know how many headers we have
    httpMethod method;
    char *body;
    int status; //Code returned on the http method
};

void fetch(char* url, struct fetchParams_t *fetchParams);

#endif
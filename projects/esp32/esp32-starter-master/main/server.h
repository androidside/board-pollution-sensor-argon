#ifndef _SERVER_H_
#define _SERVER_H_


void RegisterEndPoints(void);

#endif

//* Sample Syntax *//

// void RegisterEndPoints(void);
// typedef struct reading_t
// {
//     int sensor_id;
//     float latitude;
//     float longitude;
//     char datetime[300]; //Check actual size! It could be less than 300
//     int intensity;
// } reading_t;

// void populateRandomReading(struct reading_t *reading);
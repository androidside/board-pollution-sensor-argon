#ifndef _READING_H_
#define _READING_H_

typedef struct reading_t
{
    int sensor_id;
    float latitude;
    float longitude;
    char datetime[300]; //Check actual size! It could be less than 300
    int intensity;
    float vgas;
    float vgas0;
    float temperature;
    float ppm;
    int rgain;

} reading_t;

void populateDemoReadingRandom(struct reading_t *reading);
void populateDemoReading(struct reading_t *reading);
void populateRandomReading(struct reading_t *reading);
void populateCanonicalReading(struct reading_t *reading);
void printReading(struct reading_t *reading);

#endif
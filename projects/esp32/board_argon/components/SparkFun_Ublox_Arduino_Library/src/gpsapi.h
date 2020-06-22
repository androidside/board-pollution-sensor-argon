#ifndef _GPSAPI_H
#define _GPSAPI_H





#ifdef __cplusplus
extern "C"
{
#endif

    void startGPS();
    void readGPS(float *latitude_param, float *longitude_param, char *datetime_param);
    // const char* readlatitude();
    // const char* readlongitude();
    // const char* readdatetime();

#ifdef __cplusplus
}
#endif

#endif
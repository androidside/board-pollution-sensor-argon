
#ifndef _SPARKFUNADS105API_H
#define _SPARKFUNADS105API_H

#ifdef __cplusplus
extern "C"
{
#endif
    void startADC();
    void ADS1X15_get_raw(int *differential_a0a3, int *differential_a1a3, int *differential_a2a3);
#ifdef __cplusplus
}
#endif

#endif
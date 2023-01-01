#include "FuelMap.h"
#include <Arduino.h>

unsigned char fuelmap(double hz) {
    // 1.5hz -> 24
    // 2.0hz -> 32

    // double fanspeed = 16.0 * hz;
    double fanspeed = 8.0 * pow(hz, 1.75);

    unsigned char bounded = (unsigned char)constrain((long)round(fanspeed), (long)0, (long)255);
    return bounded;
}

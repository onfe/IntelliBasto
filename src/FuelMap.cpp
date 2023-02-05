#include "FuelMap.h"
#include <Arduino.h>

// At 4.5hz, tweaking the power gives the following result:
// 1.55 is very sooty
// 1.6 is a bit sooty
// 1.65 is a bit sooty, a few pops
// 1.7 todo
// 1.75 todo
// 1.8 pops a lot then goes out.

// I think we can run more lean at higher burn rates, as the combustion chamber
// gets hotter, but I haven't tested this. The other reason we can struggle to
// run lean is due to soot on the nebuliser screen and combustion chamber.

// A solution could be to run hotter (~6-7hz) at startup then tone down to 4-4.5hz.
// This should in theory get the chamber nice and hot for at least a few minutes
// which might reduce soot buildup over time.

unsigned char fuelmap(double hz) {
    double fanspeed = 8.0 * pow(hz, 1.6);

    unsigned char bounded = (unsigned char)constrain((long)round(fanspeed), (long)0, (long)255);
    return bounded;
}

#ifndef INTELLIBASTO_FUEL_H
#define INTELLIBASTO_FUEL_H

#include "config.h"
#include "SmoothedOutput.h"


class Fuel : private SmoothedOutput {
    // gives the range of 0 - 8Hz in 1/32Hz (0.03125Hz) increments.
    static const unsigned char multiplier = 32;

    // ms the output is high for.
    static const unsigned char tickLength = 50;

    unsigned long lastTick = 0;
    
public:
    Fuel();

    virtual void set(float value);
    void update() override;
};


#endif //INTELLIBASTO_FUEL_H

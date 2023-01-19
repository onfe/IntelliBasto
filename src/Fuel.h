#ifndef INTELLIBASTO_FUEL_H
#define INTELLIBASTO_FUEL_H

#include "config.h"
#include "SmoothedOutput.hpp"


class Fuel : public SmoothedOutput {
private:
    unsigned long lastTick = 0;

    // ms the output is high for.
    static const unsigned char tickLength = 50;

public:
    // gives the range of 0 - 8Hz in 1/32Hz (0.03125Hz) increments.
    static const unsigned char multiplier = 32;

    Fuel() : SmoothedOutput(PIN_FUEL, 5000U, 0U, 255U, true), lastTick{millis()} {}

    void set(unsigned char value) override;
    virtual void set(float value);
    void ramp(unsigned char value) override;
    void ramp(float hz);
    void update() override;

};


#endif //INTELLIBASTO_FUEL_H

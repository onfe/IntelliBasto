//
// Created by Eddie on 30/04/2022.
//

#ifndef INTELLIBASTO_SMOOTHEDOUTPUT_HPP
#define INTELLIBASTO_SMOOTHEDOUTPUT_HPP

#include <Arduino.h>
#include "Output.hpp"

class SmoothedOutput : public Output {
protected:
    unsigned short smoothing;

    unsigned char from;
    unsigned char to;
    unsigned long startTime;

public:

    SmoothedOutput(unsigned char pin, unsigned short smoothing, unsigned char min = 0, unsigned char max = 255,
                   bool allowZero = true) : Output(pin, min, max, allowZero), smoothing{smoothing}, from{0}, to{0},
                                            startTime{millis()} {}

    void update() override;

    void set(unsigned char value) override;
    virtual void ramp(unsigned char value);

    virtual void rampOn();
    virtual void rampOff();

    unsigned char get() override;


};


#endif //INTELLIBASTO_SMOOTHEDOUTPUT_HPP

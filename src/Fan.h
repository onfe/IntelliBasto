#ifndef INTELLIBASTO_FAN_H
#define INTELLIBASTO_FAN_H

#include "config.h"
#include "SmoothedOutput.h"

#include <Arduino.h>

// Fan is 8v, so cap the maximum PWM value to (8/12) * 255 = 169

class Fan : public SmoothedOutput {
public:
    Fan() : SmoothedOutput(PIN_FAN, 10000U, 24U, 169U, true) {}
};

#endif //INTELLIBASTO_FAN_H

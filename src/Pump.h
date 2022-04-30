#ifndef INTELLIBASTO_PUMP_H
#define INTELLIBASTO_PUMP_H

#include "config.h"
#include "SmoothedOutput.h"

#include <Arduino.h>

class Pump : public SmoothedOutput {
public:
    Pump() : SmoothedOutput(PIN_WATER, 5000U, 32U, 255U, true) {}
};

#endif //INTELLIBASTO_PUMP_H

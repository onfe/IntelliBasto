#ifndef INTELLIBASTO_PUMP_H
#define INTELLIBASTO_PUMP_H

#include "config.h"
#include "SmoothedOutput.hpp"

#include <Arduino.h>

class Pump : public Output {
public:
    Pump() : Output(PIN_WATER) {}
};

#endif //INTELLIBASTO_PUMP_H

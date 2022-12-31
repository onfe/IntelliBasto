#ifndef INTELLIBASTO_HEATER_HPP
#define INTELLIBASTO_HEATER_HPP


#include "Glow.h"
#include "Fan.h"
#include "Pump.h"
#include "Fuel.h"
#include "Matrix.h"
#include "TempSensor.hpp"
#include "Modes.hpp"

struct Heater {
    // Inputs
    Modes modes = Modes();
    TempSensor water = TempSensor(PIN_THERM_WATER, BIAS_RESISTOR_OHMS, THERM_SH_COEFF_A, THERM_SH_COEFF_B, THERM_SH_COEFF_C);
    TempSensor exhaust = TempSensor(PIN_THERM_EXHAUST, BIAS_RESISTOR_OHMS, THERM_SH_EXHAUST_A, THERM_SH_EXHAUST_B, THERM_SH_EXHAUST_C);

    // Outputs
    Glow glow = Glow();
    Fan fan = Fan();
    Pump pump = Pump();
    Fuel fuel = Fuel();
    Matrix matrix = Matrix();

    unsigned long timeInState = millis();
    Error lastError = Error::NONE;
};


#endif //INTELLIBASTO_HEATER_HPP

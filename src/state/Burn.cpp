#include "States.hpp"
#include "FuelMap.h"

void Burn::pre(Heater &h) {
    h.glow.off();
    h.matrix.off();
    h.pump.on();

    h.fuel.ramp(3.5f);
    h.fan.ramp(fuelmap(3.5f));
}

StateResult Burn::run(Heater &h) {
    if (h.water.temp() > TEMP_UPPER || h.modes.get() == Mode::OFF) {
        return StateResult(Error::NONE, NextState::EXTINGUISH);
    }

    if (h.exhaust.temp() > TEMP_EXHAUST_MAXIMUM) {
        return StateResult(Error::MINOR, NextState::EXTINGUISH);
    }

    if (h.exhaust.decreasing() || h.exhaust.temp() < TEMP_EXHAUST_MINIMUM) {
        return StateResult(Error::MINOR, NextState::EXTINGUISH);
    }

    // todo!
    return StateResult(Error::NONE);
}

void Burn::post(Heater &h) {
    h.glow.off();
    h.matrix.rampOff();
    h.pump.on();

    h.fuel.off();
}

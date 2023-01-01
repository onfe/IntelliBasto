#include "States.hpp"
#include "FuelMap.h"

void PostIgnition::pre(Heater &h) {
    h.glow.on();
    h.matrix.off();
    h.pump.on();

    float fuel = (float)h.fuel.get() / (float)Fuel::multiplier;

    if (fuel < 3.5f) {
        fuel = 3.5f;
    }

    h.fuel.ramp(fuel);
    h.fan.ramp(fuelmap(fuel));
}

StateResult PostIgnition::run(Heater &h) {
    if (h.exhaust.temp() < TEMP_EXHAUST_MINIMUM || h.exhaust.decreasing()) {
        return StateResult(Error::MINOR, NextState::EXTINGUISH);
    }

    if (h.exhaust.temp() > TEMP_EXHAUST_MAXIMUM) {
        return StateResult(Error::MINOR, NextState::EXTINGUISH);
    }

    if (h.timeInState > 15 * 1000) {
        h.glow.rampOff();
    }

    if (h.timeInState > 30 * 1000) {
        return StateResult(Error::NONE, NextState::BURN);
    }

    // It's best for the heater to burn for a bit after lighting (to prevent soot buildup) but if the water's too hot
    // we need to stop here.
    if (h.water.temp() > TEMP_UPPER) {
        return StateResult(Error::NONE, NextState::EXTINGUISH);
    }

    return StateResult(Error::NONE);
}

void PostIgnition::post(Heater &h) {
    h.glow.off();
    h.fan.off();
    h.matrix.off();
    h.fuel.off();
    h.pump.off();
}

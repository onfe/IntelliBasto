#include "States.hpp"
#include "FuelMap.h"

void Burn::pre(Heater &h) {
    h.glow.off();
    h.matrix.off();
    h.pump.on();

    maxTempReached = h.exhaust.temp();

    h.fuel.ramp(4.0f);
    h.fan.ramp(fuelmap(4.0f));
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

    float et = h.exhaust.temp();
    if (et > maxTempReached) {
        maxTempReached = et;
    }

    // if temp drops for some reason.
    if (et < maxTempReached - 20) {
        return StateResult(Error::MINOR, NextState::EXTINGUISH);
    }


    if (et < maxTempReached - 10 && et < TEMP_EXHAUST_MINIMUM + 50) {
        return StateResult(Error::MINOR, NextState::EXTINGUISH);
    }

    // Handle Matrix
    if (h.modes.get() == Mode::HEAT_S1) {
        if (h.water.temp() > TEMP_SETPOINT) {
            h.matrix.rampOn();
        } else if (h.water.temp() < TEMP_SETPOINT - 1) {
            h.matrix.rampOff();
        }

    } else if (h.modes.get() == Mode::HEAT_S2) {
        if (h.water.temp() > 45) {
            h.matrix.rampOn();
        } else if (h.water.temp() < 40) {
            h.matrix.rampOff();
        }
    } else {
        // shouldn't be possible, but here in case prime gets activated, for example.
        return StateResult(Error::MINOR, NextState::EXTINGUISH);
    }

    return StateResult(Error::NONE);
}

void Burn::post(Heater &h) {
    h.glow.off();
    h.matrix.rampOff();
    h.pump.on();

    h.fuel.off();
}

#include "States.hpp"
#include "FuelMap.h"

void PostIgnition::pre(Heater &h) {
    h.glow.on();
    h.pump.on();

    float fuel = (float)h.fuel.get() / (float)Fuel::multiplier;

    if (fuel < 3.5f) {
        fuel = 3.5f;
    }

    h.fuel.ramp(fuel);
    h.fan.ramp(fuelmap(fuel));
}

StateResult PostIgnition::run(Heater &h) {
    if (h.exhaust.temp() < 50 || h.exhaust.decreasing()) {
        Serial.println("Temp decreasing.");
        return StateResult(Error::MINOR, NextState::EXTINGUISH);
    }

    if (h.exhaust.temp() > TEMP_EXHAUST_MAXIMUM) {
        Serial.println("Temp too high.");
        return StateResult(Error::MINOR, NextState::EXTINGUISH);
    }

    if (h.timeInState > 15U * 1000U) {
        h.glow.rampOff();
    }

    if (h.exhaust.temp() > TEMP_EXHAUST_MINIMUM + 10) {
        return StateResult(Error::NONE, NextState::BURN);
    }

    if (h.timeInState > 90U * 1000U) {

        if (h.exhaust.temp() < TEMP_EXHAUST_MINIMUM) {
            Serial.println("Temp didn't rise enough.");
            return StateResult(Error::MINOR, NextState::EXTINGUISH);
        }

        // if we had a failed ignition error stored, as we've now lit successfully, clear it.
        if (h.lastError == Error::FAILED_TO_LIGHT) {
            return StateResult(Error::RESET, NextState::BURN);
        }

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
    h.pump.off();
}

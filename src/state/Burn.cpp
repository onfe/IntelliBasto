#include "States.hpp"
#include "FuelMap.h"

void Burn::pre(Heater &h) {
    h.glow.off();
    h.pump.on();

    maxTempReached = h.exhaust.temp();

    h.fuel.ramp(4.5f);
    h.fan.ramp(fuelmap(4.5f));
}

StateResult Burn::run(Heater &h) {
    if (h.water.temp() > TEMP_UPPER || h.modes.get() == Mode::OFF) {
        return StateResult(Error::NONE, NextState::EXTINGUISH);
    }

    if (h.exhaust.temp() > TEMP_EXHAUST_MAXIMUM) {
        Serial.println("too hot.");
        return StateResult(Error::MINOR, NextState::EXTINGUISH);
    }

    if (h.exhaust.decreasing() || h.exhaust.temp() < TEMP_EXHAUST_MINIMUM) {
        Serial.println("decrease or too low");
        return StateResult(Error::MINOR, NextState::EXTINGUISH);
    }

    float et = h.exhaust.temp();
    if (et > maxTempReached) {
        maxTempReached = et;
    }

    // if temp drops for some reason.
    if (et < maxTempReached - 25) {
        Serial.println("drop 25");
        return StateResult(Error::MINOR, NextState::EXTINGUISH);
    }


    if (et < maxTempReached - 10 && et < TEMP_EXHAUST_MINIMUM + 50) {
        Serial.println("drop 10");
        return StateResult(Error::MINOR, NextState::EXTINGUISH);
    }

    return StateResult(Error::NONE);
}

void Burn::post(Heater &h) {
    h.glow.off();
    h.pump.on();

    h.fuel.off();
}

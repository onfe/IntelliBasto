#include "States.hpp"
#include "config.h"
#include "FuelMap.h"

void Ignition::pre(Heater &h) {
    h.glow.on();
    h.fan.set(24);
    h.fuel.set(1.0f);
    h.pump.on();
}

StateResult Ignition::run(Heater &h) {
    h.glow.on();
    h.pump.on();

    if (h.timeInState > 15U * 1000U && h.exhaust.temp() > 60 && h.exhaust.increasing()) {
        return StateResult(Error::NONE, NextState::POST_IGNITION);
    }

    if (h.timeInState > 90UL * 1000UL) {
        if (h.lastError == Error::FAILED_TO_LIGHT) {
            return StateResult(Error::MINOR, NextState::EXTINGUISH);
        } else {
            return StateResult(Error::FAILED_TO_LIGHT, NextState::EXTINGUISH);
        }
    }

   // Start off with a blast of fuel to get the burn chamber primed.
   if (h.timeInState < 2 * 1000) {
       h.fuel.set(6.0f);
       h.fan.set(24);

       return StateResult(Error::NONE);
   }

   if (h.timeInState < 10U * 1000) {
       h.fan.set(24);
       h.fuel.off();

       return StateResult(Error::NONE);
   }

    if (h.timeInState < 15U * 1000U) {
        h.fan.set(32);
        h.fuel.set(1.0f);

        return StateResult(Error::NONE);
    }

    if (h.timeInState < 30U * 1000U) {
        h.fan.ramp(32);
        h.fuel.ramp(2.5f);

        return StateResult(Error::NONE);
    }

    h.fuel.ramp(3.5f);
    h.fan.ramp(fuelmap(3.5f));

    // float fuel = 1.5f + ((float)h.timeInState / (float)(30 * 1000));
    // h.fuel.set(fuel);
    // h.fan.set(fuelmap(fuel));

    return StateResult(Error::NONE);
}

void Ignition::post(Heater &h) {

}

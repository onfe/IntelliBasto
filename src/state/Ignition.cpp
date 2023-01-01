#include "States.hpp"
#include "config.h"
#include "FuelMap.h"

void Ignition::pre(Heater &h) {
    h.glow.on();
    h.fan.set(24);
    h.matrix.off();
    h.fuel.set(4.0f);
    h.pump.on();
}

StateResult Ignition::run(Heater &h) {
    h.glow.on();
    h.pump.on();

    // Start off with a blast of fuel to get the burn chamber primed.
    if (h.timeInState < 2 * 1000) {
        h.fuel.set(4.0f);
        h.fan.set(24);

        return StateResult(Error::NONE);
    }

    float fuel = 2.5f + ((float)h.timeInState / (float)(30 * 1000));
    h.fuel.ramp(fuel);
    h.fan.ramp(fuelmap(fuel));


    if (h.timeInState > 15 * 1000 && h.exhaust.temp() > TEMP_EXHAUST_MINIMUM && h.exhaust.increasing()) {
        return StateResult(Error::NONE, NextState::POST_IGNITION);
    }

    if (h.timeInState > 60U * 1000) {
        return StateResult(Error::FAILED_TO_LIGHT, NextState::EXTINGUISH);
    }

    return StateResult(Error::NONE);
}

void Ignition::post(Heater &h) {

}

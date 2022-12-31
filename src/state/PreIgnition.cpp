#include "PreIgnition.hpp"

void PreIgnition::pre(Heater &h) {
    h.glow.off();
    h.fan.set(92);
    h.matrix.off();
    h.fuel.off();
    h.pump.on();
}

StateResult PreIgnition::run(Heater &h) {
    // if heating is no longer required, return to idle.
    if (!(h.modes.get() == Mode::HEAT_S1 || h.modes.get() == Mode::HEAT_S2)) {
        return StateResult(Error::NONE, NextState::IDLE);
    }

    if (h.timeInState > 10 * 1000) {
        h.fan.ramp(32);
        h.glow.rampOn();
    }

    if (h.timeInState > 30U * 1000U) {
        return StateResult(Error::NONE, NextState::IGNITION);
    }

    return StateResult(Error::NONE);
}

void PreIgnition::post(Heater &h) {
    h.glow.off(); // sanity check to ensure glowplug doesn't get left on.
    h.fan.ramp(32);

    h.matrix.off();
    h.fuel.off();
    h.pump.on();
}

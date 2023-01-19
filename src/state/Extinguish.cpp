#include "States.hpp"

void Extinguish::pre(Heater &h) {
    h.glow.off();
    h.fan.ramp(92);
    h.matrix.off();
    h.fuel.off();
    h.pump.on();
}

StateResult Extinguish::run(Heater &h) {
    if (h.timeInState > 20U * 1000) {
        h.glow.rampOn();
    }

    if (h.timeInState > 40U * 1000) {
        return StateResult(Error::NONE, NextState::COOL_DOWN);
    }

    return StateResult(Error::NONE);
}

void Extinguish::post(Heater &h) {
    h.glow.off(); // sanity check to ensure glowplug doesn't get left on.
    h.fan.rampOff();
    h.matrix.off();
    h.fuel.off();
    h.pump.on();
}

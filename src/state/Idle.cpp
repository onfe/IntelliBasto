#include "States.hpp"
#include "config.h"

void Idle::pre(Heater &h) {
    h.glow.off();
    h.fan.rampOff();
    h.matrix.rampOff();
    h.fuel.off();
    h.pump.off();
}

StateResult Idle::run(Heater &h) {


    if ((h.modes.get() == Mode::HEAT_S1 || h.modes.get() == Mode::HEAT_S1) && h.timeInState > MIN_TIME_IDLE) {
        return StateResult(Error::NONE, NextState::CIRCULATE);
    }

    // the heater block has a lot of thermal mass, so sometimes after the heater has turned off the water that's
    // currently in the heater block can get too hot. If this happens we need to turn the pump on to dissipate the heat.
    if (h.water.temp() > TEMP_UPPER) {
        h.pump.on();
    } else {
        h.pump.off();
    }

    if (h.modes.get() == Mode::PRIME) {
        h.fuel.ramp(4.0f);
    } else {
        h.fuel.off();
    }


    return StateResult(Error::NONE);
}

void Idle::post(Heater &h) {
    h.glow.off();
    h.fan.off();
    h.matrix.off();
    h.fuel.off();
    h.pump.off();
}

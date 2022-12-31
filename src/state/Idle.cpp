#include "Idle.hpp"
#include "config.h"

void Idle::pre(Heater &h) {
    h.glow.off();
    h.fan.off();
    h.matrix.off();
    h.fuel.off();
    h.pump.off();
}

StateResult Idle::run(Heater &h) {


    if ((h.modes.get() == Mode::HEAT_S1 || h.modes.get() == Mode::HEAT_S1) && h.timeInState > MIN_TIME_IDLE) {
        return StateResult(Error::NONE, NextState::CIRCULATE);
    }

    if (h.water.temp() > TEMP_UPPER) {
        h.pump.on();
    }

    if (h.modes.get() == Mode::PRIME) {
        return StateResult(Error::NONE, NextState::PRIME);
    }


    return StateResult(Error::NONE);
}

void Idle::post(Heater &h) {

}

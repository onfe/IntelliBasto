#include "States.hpp"

void CoolDown::pre(Heater &h) {
    h.glow.off();
    h.fan.ramp(64);
    h.fuel.off();
    h.pump.on();
}

StateResult CoolDown::run(Heater &h) {
    if (h.timeInState > 60U * 1000U && h.water.temp() < TEMP_UPPER && h.exhaust.temp() < 75) {

        if (h.lastError != Error::NONE && h.lastError != Error::FAILED_TO_LIGHT) {
            return StateResult(Error::NONE, NextState::FAIL);
        }

        return StateResult(Error::NONE, NextState::IDLE);
    }

    return StateResult(Error::NONE);
}

void CoolDown::post(Heater &h) {
    h.glow.off();
    h.fan.off();
    h.fuel.off();
    h.pump.off();
}

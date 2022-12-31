#include "Circulate.hpp"

void Circulate::pre(Heater &h) {
    h.glow.off();
    h.matrix.off();
    h.fuel.off();
    h.fan.off();

    h.pump.on();
}

StateResult Circulate::run(Heater &h) {
    bool heatRequested = h.modes.get() == Mode::HEAT_S1 || h.modes.get() == Mode::HEAT_S2;
    bool heatRequired = h.water.temp() < TEMP_LOWER;

    if (!heatRequested) {
        return StateResult(Error::NONE, NextState::IDLE);
    }

    if (h.timeInState > MIN_TIME_CIRCULATE && heatRequired) {
        return StateResult(Error::NONE, NextState::PRE_IGNITION);
    }

    return StateResult(Error::NONE);
}

void Circulate::post(Heater &h) {

}
#include "States.hpp"

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

    // if the water is too hot, keep the pump going to dissipate the heat.
    if (h.water.temp() > TEMP_UPPER) {
        return StateResult(Error::NONE);
    }

    // if no heat is requested, return to idle.
    if (!heatRequested) {
        return StateResult(Error::NONE, NextState::IDLE);
    }

    // if heat is requested, and the water temp is lower than the setpoint, we therefore need heat.
    // wait until we've circulated for enough time to get an accurate water temp reading.
    if (h.timeInState > MIN_TIME_CIRCULATE && heatRequired) {
        return StateResult(Error::NONE, NextState::PRE_IGNITION);
    }

    return StateResult(Error::NONE);
}

void Circulate::post(Heater &h) {

}

#include "States.hpp"

void Failure::pre(Heater &h) {
    h.glow.off();
    h.fan.ramp(32);
    h.fuel.off();
    h.pump.on();
}

StateResult Failure::run(Heater &h) {
    if (h.lastError == Error::FAILED_TO_LIGHT) {
        Serial.println("This error (failure to light) shouldn't have triggered failure. Escalating...");
        return StateResult(Error::MINOR);
    }

    if (h.lastError == Error::NONE) {
        Serial.println("No error logged yet error state reached. Escalating...");
        return StateResult(Error::MINOR);
    }

    // Minimum 60s in this state.
    if (h.timeInState < (60U * 1000)) {
        return StateResult(Error::NONE);
    }

    if (h.water.temp() < TEMP_UPPER && h.exhaust.temp() < 100) {
        h.fan.rampOff();
        h.pump.off();

        // Minor errors get cleared when the thermostat is no longer calling for heat.
        if (h.modes.get() == Mode::OFF && h.lastError == Error::MINOR) {
            return StateResult(Error::RESET, NextState::IDLE);
        }
    } else {
        // if we've been in this state for over 10 minutes and the temperatures are still too high, it's likely a
        // sensor error, so we stop the fans and pump but escalate to major error.
        if (h.timeInState > (unsigned long)(600UL * 1000UL)) {
            h.fan.rampOff();
            h.pump.off();
            return StateResult(Error::MAJOR);
        } else {
            h.fan.ramp(32);
            h.pump.on();
        }
    }

    // Major errors will never be cleared, a power cycle is needed.
    return StateResult(Error::NONE);
}

void Failure::post(Heater &h) {
    h.glow.off();
    h.fan.rampOff();
    h.fuel.off();
    h.pump.off();
}

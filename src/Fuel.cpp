#include "Fuel.h"

#include <Arduino.h>

void Fuel::update() {
    float hz = (float)get() / (float)multiplier;

    // if Hz is zero, end the tick and return early, preventing a divide by zero.
    if (hz == 0) {
        digitalWrite(pin, LOW);
        return;
    }

    // Calculate the milliseconds between ticks.
    auto msTick = (unsigned long)(1000.0 / hz);

    // if it's been long enough since the last tick, start another.
    if (millis() - lastTick > msTick) {
        lastTick = millis();
        digitalWrite(pin, HIGH);

        return;
    }

    // if the current tick has been going on for > tickLength, turn off the output.
    if (millis() - lastTick > tickLength) {
        digitalWrite(pin, LOW);
        return;
    }
}

void Fuel::set(float hz) {
    // Constrain the hz to be within the range offered by the multiplier.
    hz = constrain(hz, (float)min, ((float)max / (float)multiplier));
    SmoothedOutput::set((unsigned char)(hz * multiplier));
}

void Fuel::set(unsigned char value) {
    this->set((float)value);
}

void Fuel::ramp(float hz) {
    // Constrain the hz to be within the range offered by the multiplier.
    hz = constrain(hz, (float)min, ((float)max / (float)multiplier));
    SmoothedOutput::ramp((unsigned char)(hz * multiplier));
}

void Fuel::ramp(unsigned char value) {
    this->ramp((float)value);
}

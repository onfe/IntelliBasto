//
// Created by Eddie on 30/04/2022.
//

#include "SmoothedOutput.hpp"

void SmoothedOutput::update() {
    analogWrite(pin, get());
}

void SmoothedOutput::ramp(unsigned char value) {
    if (value != 0 || !allowZero)
        value = constrain(value, this->min, this->max);

    if (value == to) return;

    this->from = constrain(get(), min, max);
    this->to = value;
    this->startTime = millis();

    this->update();
}

void SmoothedOutput::set(unsigned char value) {
    if (value != 0 || !allowZero)
        value = constrain(value, this->min, this->max);

    this->from = value;
    this->to = value;
    this->startTime = millis();

    this->update();
}

unsigned char SmoothedOutput::get() {
    unsigned long timeSinceStart = millis() - startTime;

    // return if there's no smoothing, or if `to` and `from` are the same.
    if (from == to || smoothing < 1) return to;

    // if the smoothing has already been done, return to & set from == to
    // to prevent issues when millis() overflows.
    if (timeSinceStart >= smoothing) {
        from = to;
        return to;
    }

    // multiply the change in value needed with the percent of the duration that has passed.
    float perc = (float)timeSinceStart / (float)smoothing;
    int cTo = constrain(to, min, max);
    int out = from + (int)((cTo - from) * perc);

    if (to == 0 && out < min && allowZero) {
        out = 0;
        from = 0;
    } else {
        out = constrain(out, min, max);
    }

    return out;
}

void SmoothedOutput::rampOn() {
    ramp(255);
}

void SmoothedOutput::rampOff() {
    ramp(0);
}

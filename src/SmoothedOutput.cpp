//
// Created by Eddie on 30/04/2022.
//

#include "SmoothedOutput.h"

#include <Arduino.h>

SmoothedOutput::SmoothedOutput(unsigned char pin, unsigned short smoothing, unsigned char min = 0, unsigned char max = 255, bool allowZero = true) {
    this->pin = pin;
    this->smoothing = smoothing;
    this->min = min;
    this->max = max;

    this->from = 0;
    this->to = 0;
    this->startTime = millis();

    this->allowZero = allowZero;
}

void SmoothedOutput::update() {
    analogWrite(pin, get());
}

void SmoothedOutput::set(unsigned char value) {
    if (value == to) return;

    if (value != 0 || !allowZero)
        value = constrain(value, this->min, this->max);

    this->from = constrain(get(), min, max);
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
    int out = from + ((cTo - from) * perc);

    if (to == 0 && out < min && allowZero) {
        out = 0;
        from = 0;
    } else {
        out = constrain(out, min, max);
    }

    return out;
}
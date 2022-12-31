#include "Modes.hpp"
#include "config.h"

#include <Arduino.h>

Modes::Modes() {
    lastCheck = millis();
    lastModeChange = millis();
}

void Modes::update() {
    if (millis() - lastCheck < checkInterval) return;
    lastCheck = millis();

    // ye-olde bit field
    unsigned char value = !digitalRead(PIN_RFH_1) | (!digitalRead(PIN_RFH_2) << 1 | digitalRead(PIN_PRIME) << 2);
    inputs[index] = value;

    ++index;
    if (index >= sizeof(inputs) / sizeof(inputs[0])) {
        index = 0;
    }

    if (millis() - lastModeChange < MODE_CHANGE_DELAY) return;

    // debouncing
    unsigned char inp0 = inputs[0];
    for (int i = 1; i < sizeof(inputs) / sizeof(inputs[0]); ++i) {
        if (inputs[0] != inputs[i]) return;
    }

    Mode output;

    if (inp0 == 255) return;

    if (inp0 == 0x01) {
        output = Mode::PRIME;
    } else if (inp0 & 0x02) {
        output = Mode::HEAT_S2;
    } else if (inp0 & 0x01) {
        output = Mode::HEAT_S1;
    } else {
        output = Mode::OFF;
    }

    if (output != currentMode) {
        lastModeChange = millis();
        currentMode = output;
    }
}

void Modes::init() {
    pinMode(PIN_RFH_1, INPUT_PULLUP);
    pinMode(PIN_RFH_2, INPUT_PULLUP);
    pinMode(PIN_PRIME, INPUT_PULLUP);
}

Mode Modes::get() {
    return currentMode;
}

const char * Modes::getString() {
    Mode m = get();
    switch (m) {
        case Mode::OFF:
            return "OFF";
        case Mode::HEAT_S1:
            return "H-1";
        case Mode::HEAT_S2:
            return "H-2";
        case Mode::PRIME:
            return "PRM";
    }
}

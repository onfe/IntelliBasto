#include "Modes.h"
#include "config.h"

#include <Arduino.h>

Modes::Modes() {
    lastCheck = millis();
    lastModeChange = millis();
}

void Modes::update() {
    if (millis() - lastCheck < checkInterval) return;
    lastCheck = millis();

    // ye-olde bit field, 0 = off, 1 = stage 1, 2 = stage 2, 3 = both stages.
    unsigned char value = !digitalRead(PIN_RFH_1) | (!digitalRead(PIN_RFH_2) << 1);
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

    if (inp0 > 1) {
        output = HEAT_S2;
    } else if (inp0 > 0) {
        output = HEAT_S1;
    } else {
        output = OFF;
    }

    if (output != currentMode) {
        lastModeChange = millis();
        currentMode = output;
    }
}

Mode Modes::get() {
    return currentMode;
}
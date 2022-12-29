#include "Glow.h"

#include <Arduino.h>

void Glow::update() {
    unsigned char value = get();

    on = value > 0;

    // if the glow plug is off, record that it's off now.
    if (!on) {        
        timeLastOff = millis();
    }

    // if the glowplug has been on for longer than it should, turn it off.
    // if (millis() - timeLastOff > maxOnTime && on) {
    //     analogWrite(pin, 0);
    //     Serial.println("The Glow Plug has been on for too long! Turning off...");
    //     return;
    // }

    analogWrite(pin, value);
}
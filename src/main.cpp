#include <Arduino.h>
#include "config.h"
#include "TempCtrl.h"

TempCtrl temp;

void setup() {
    Serial.begin(9600);
    // Initialise the pins required.
    pinMode(PIN_MATRIX, OUTPUT);
    pinMode(PIN_FUEL, OUTPUT);
    pinMode(PIN_WATER, OUTPUT);
    pinMode(PIN_FAN, OUTPUT);
    pinMode(PIN_GLOW, OUTPUT);

    pinMode(PIN_RFH_1, INPUT_PULLUP);
    pinMode(PIN_RFH_2, INPUT_PULLUP);
    // Don't need to set the pinMode for analogue pins.

    pinMode(LED_BUILTIN, OUTPUT);
    // Initialise any classes.
}

void loop() {
    Status err;
    err = temp.update();
    int water = temp.water();
    int exhaust = temp.exhaust();

    char str[64];
    sprintf(str, "water: %d, exhaust: %d, err?: %d\n", water, exhaust, err);
    Serial.print(str);


    digitalWrite(LED_BUILTIN, 0);
    delay(100);
    digitalWrite(LED_BUILTIN, 1);
    delay(100);

}
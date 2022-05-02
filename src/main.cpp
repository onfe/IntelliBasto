#include <Arduino.h>
#include "config.h"
#include "TempCtrl.h"
#include "Matrix.h"
#include "Fuel.h"
#include "Glow.h"


TempCtrl temp;
Matrix matrix;
Fuel fuel;
Glow glow;

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

    matrix.set(0);
    fuel.set(0);
}

void loop() {
    Status err;
    err = temp.update();
    int water = temp.water();
    int exhaust = temp.exhaust();
    
    Serial.print(digitalRead(PIN_RFH_1));

    // char str[64];
    // sprintf(str, "water: %d, exhaust: %d, err?: %d\n", water, exhaust, err);
    // Serial.print(str);

    matrix.update();
    fuel.update();
    glow.update();

    if ((millis() / 15000) % 2 == 0) {
        // matrix.set(255);
        glow.set(127);
    } else {
        // matrix.set(0);
        glow.set(0);
    }

    delay(10);

}
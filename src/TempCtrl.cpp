#include "TempCtrl.h"

#include <Arduino.h>

TempCtrl::TempCtrl() = default;

Status TempCtrl::update() {
    // Only check the temp 100 times per second max.
    if (millis() - lastUpdate < 10) return OK;

    // Read the ADC voltage from each sensor.
    int exhaust = getTemp(PIN_THERM_EXHAUST, true);
    if (exhaust < -15 || exhaust > 500) return ERROR;

    int water = getTemp(PIN_THERM_WATER, false);
    // Serial.println(water);
    if (water < -15 || water > 500) return ERROR;

    this->exhaustTemps[index] = exhaust;
    this->waterTemps[index] = water;

    ++index;
    if (index >= sizeof(exhaustTemps) / sizeof(exhaustTemps[0])) {
        index = 0;
        filled = true;
    }
    return OK;
}

int TempCtrl::getTemp(int pin, bool exhaust) {
    int adc = analogRead(pin);

    // calculate the resistance based on the adc value and the bias resistor.
    double resistance = (double)BIAS_RESISTOR_OHMS / (1023.0 / adc - 1);
    // if (!exhaust && millis() % 100 == 0) {
    //     Serial.println(resistance);
    // }

    // Steinhart-Hart Thermistor Equation
    int kelvin = 0;
    if (!exhaust) {
        kelvin = steinhart(resistance, THERM_SH_COEFF_A, THERM_SH_COEFF_B, THERM_SH_COEFF_C);
    } else {
        kelvin = steinhart(resistance, THERM_SH_EXHAUST_A, THERM_SH_EXHAUST_B, THERM_SH_EXHAUST_C);
    }
    return kelvin - 273;
}

int TempCtrl::steinhart(double r, double a, double b, double c) {
    double logR = log((double)r);

    return 1.0 / (a + (b * logR) + (c * logR * logR * logR));
}

int TempCtrl::computeAverage(int arr[], int size) {
    int total = 0;
    if (size < 1) return 0;

    for (int i = 0; i < size; ++i) {
        total += arr[i];
    }

    return total / size;
}

int TempCtrl::water() {
    int size = filled ? sizeof(waterTemps) / sizeof(waterTemps[0]) : index;
    return computeAverage(waterTemps, size);
}

int TempCtrl::exhaust() {
    int size = filled ? sizeof(exhaustTemps) / sizeof(exhaustTemps[0]) : index;
    return computeAverage(exhaustTemps, size);
}
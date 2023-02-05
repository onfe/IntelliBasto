#include "TempSensor.hpp"
#include "config.h"

void TempSensor::update() {
    // There's not much point reading a value from the ADC more frequently than 40 times a second as the heater temps
    // don't fluctuate quickly, and reading less frequently makes it easier to average over time.
    if (millis() - lastUpdate < TS_SAMPLE_RATE) return;
    lastUpdate = millis();

    float temp = calculate();

    if (temp > 500 || temp < -15) {
        discarded++;
        if (discarded <= TS_MAX_DISCARDS) {
            return;
        }
    } else {
        discarded = 0;
    }

    // store the temperature as an integer multiplied by 10, to give up 1dp of resolution.
    float temp10 = temp * 10;
    this->temps[tempIndex] = round(temp10);

    ++tempIndex;
    if (tempIndex >= TS_TEMP_SIZE) {
        tempIndex = 0;
        tempFilled = true;
    }

    // Every time we've looped all the way through, add a temp to the history.
    if (tempIndex % (TS_TEMP_SIZE / 2) == 0) {
        temp10 = this->temp() * 10;

        // shuffle the folks along.
        for (int i = TS_HIST_SIZE - 1; i > 0; i--) {
            this->history[i] = this->history[i - 1];
        }

        this->history[0] = round(temp10);
    }
}

float TempSensor::calculate() {
    int adc = analogRead(this->pin);

    // calculate the resistance based on the adc value and the pullup resistor.
    double resistance = (double)pullup / (1023.0 / adc - 1);

    // Steinhart-Hart Thermistor Equation
    float kelvin = steinhart(resistance);

    return kelvin - 273.0f;
}

float TempSensor::steinhart(double resistance) const {
    double logR = log(resistance);

    return (float)(1.0 / (sh_A + (sh_B * logR) + (sh_C * logR * logR * logR)));
}

float TempSensor::temp() {
    int size = tempFilled ? TS_TEMP_SIZE : tempIndex;

    float t10 = average(temps, size);
    return t10 / 10.0f;
}

int TempSensor::average(const int *arr, int size) {
    long total = 0;
    if (size < 1) return 0;

    for (int i = 0; i < size; ++i) {
        total += arr[i];
    }

    return total / size;
}

double TempSensor::rateOfChange() {

    double X = TS_HIST_TIME / 2.0;
    double Y = average(history, TS_HIST_SIZE) / 10.0;

    double top = 0;
    double bottom = 0;
    // Serial.print("[");


    for (int i = 0; i < TS_HIST_SIZE; i++) {
        double x = ((TS_TEMP_TIME / 2) * i) - X;
        double y = (history[i] / 10.0) - Y;

        // if (this->pin == PIN_THERM_WATER) {
        //             Serial.print(history[i]);
        //             Serial.print(", ");
        // }

        top += x * y;
        bottom += x * x;
    }

    // Serial.print("]");

    return (top / bottom) * -1000.0;
}

bool TempSensor::increasing() {
    return rateOfChange() > delta;
}

bool TempSensor::decreasing() {
    return rateOfChange() < -delta;
}

bool TempSensor::stable() {
    double r = rateOfChange();
    return abs(r) < delta;
}

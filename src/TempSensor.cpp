#include "TempSensor.hpp"

void TempSensor::update() {
    // There's not much point reading a value from the ADC more frequently than 40 times a second as the heater temps
    // don't fluctuate quickly, and reading less frequently makes it easier to average over time.
    if (millis() - lastUpdate < 25) return;
    lastUpdate = millis();

    float temp = calculate();

    if (temp > 500 || temp < -15) {
        Serial.println(temp);
        discarded++;
        if (discarded <= 5) {

            return;
        }
    } else {
        discarded = 0;
    }

    // store the temperature as an integer multiplied by 10, to give up 1dp of resolution.
    float temp10 = temp * 10;
    this->temps[tempIndex] = round(temp10);

    ++tempIndex;
    if (tempIndex >= 64) {
        tempIndex = 0;
        tempFilled = true;
    }

    // Every time we've looped all the way through, add a temp to the history.
    if (tempIndex % 16 == 0) {
        temp10 = this->temp() * 10;

        // shuffle the folks along.
        for (int i = 1; i < 64; i++) {
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
    int size = tempFilled ? 64 : tempIndex;

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

bool TempSensor::increasing() {
    // int size = histFilled ? 16 : histIndex;
    // if (size < 2) {
    //     return false;
    // }

    // int size = 16;

    // int newer = average(history, 4);
    // int older = average(history + 4, 12);

    // return older + (delta * 10) < newer;
    return rateOfChange() > 0.5;
}

float TempSensor::rateOfChange() {
    float total = 0;
    const unsigned char size = 64;
    for (unsigned char i = 0; i < size - 1; i++) {
        total += (this->history[i] - this->history[i + 1]);
    }

    return total / (float)(size - 1) * 2.5f;
}

bool TempSensor::decreasing() {
    // int size = histFilled ? 16 : histIndex;
    // if (size < 2) {
    //     return false;
    // }

    // int size = 16;

    // int newer = average(history, 4); // first 4
    // int older = average(history + 8, 8); // next 4

    // return older - (delta * 10) > newer;
    return rateOfChange() < -0.5;
}

bool TempSensor::stable() {
    return !(increasing() || decreasing());
}

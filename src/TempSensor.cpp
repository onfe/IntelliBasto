#include "TempSensor.hpp"

void TempSensor::update() {
    // There's not much point reading a value from the ADC more frequently than 20 times a second as the heater temps
    // don't fluctuate quickly, and reading less frequently makes it easier to average over time.
    if (millis() - lastUpdate < 50) return;
    lastUpdate = millis();

    float temp = calculate();

    if (temp > 500 || temp < -15) {
        Serial.println(temp);
    }

    // store the temperature as an integer multiplied by 10, to give up 1dp of resolution.
    float temp10 = temp * 10;
    this->temps[tempIndex] = round(temp10);

    ++tempIndex;
    if (tempIndex >= 32) {
        tempIndex = 0;
        tempFilled = true;
    }

    // Every time we've looped all the way through, add a temp to the history.
    if (tempIndex == 0) {
        temp10 = this->temp() * 10;

        this->history[histIndex] = round(temp10);

        ++histIndex;
        if (histIndex >= 16) {
            histIndex = 0;
            histFilled = true;
        }
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
    int size = tempFilled ? 32 : tempIndex;

    float t10 = average(temps, size);
    return t10 / 10.0f;
}

int TempSensor::average(const int *arr, int size) {
    int total = 0;
    if (size < 1) return 0;

    for (int i = 0; i < size; ++i) {
        total += arr[i];
    }

    return total / size;
}

bool TempSensor::increasing() {
    int size = histFilled ? 16 : histIndex;
    if (size < 2) {
        return false;
    }

    int older = average(history, size / 2);
    int newer = average(history + (size / 2), size - (size / 2));

    return older + (delta * 10) < newer;
}

bool TempSensor::decreasing() {
    int size = histFilled ? 16 : histIndex;
    if (size < 2) {
        return false;
    }

    int older = average(history, size / 2);
    int newer = average(history + (size / 2), size - (size / 2));

    return older - (delta * 10) > newer;
}

bool TempSensor::stable() {
    return !(increasing() || decreasing());
}

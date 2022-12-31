#ifndef INTELLIBASTO_BURNCTRL_H
#define INTELLIBASTO_BURNCTRL_H

#include <Arduino.h>
#include "config.h"

class BurnCtrl
{
private:
    unsigned long last_calculate;
    int setpoint;
    double Kp;
    unsigned int dt;

    double last_result;
    double prev_exhaust[3] = {0, 0, 0};
    int exhaust_index = 0;
    bool exhaust_decreasing = false;

public:
    BurnCtrl();

    double calculate(int curr, int exhaust);
    bool lit() { return !exhaust_decreasing; }
    void reset()
    {
        last_result = 2.5;
        prev_exhaust[0] = 0.0;
        prev_exhaust[1] = 0.0;
        prev_exhaust[2] = 0.0;
    }
};

BurnCtrl::BurnCtrl()
{
    this->Kp = PID_Kp;
    this->setpoint = TEMP_SETPOINT;
    this->last_calculate = millis();
    this->last_result = 0.0;
    this->dt = PID_DT;
}

// returns the desired fuelling rate given the current error.
double BurnCtrl::calculate(int curr, int exhaust)
{
    if (last_calculate + dt > millis())
    {
        return last_result;
    }

    last_calculate = millis();

    int error = setpoint - curr;

    double Pout = Kp * error;

    double result = Pout;

    if (exhaust > 275) {
        result = last_result - 0.1;
    } else if (exhaust > 260) {
        result = last_result - 0.05;
    } else if (exhaust > 250) {
        result = last_result;
    }

    if (exhaust < 200 && result <= last_result) {
        result = last_result + 0.02;
    }

    result = constrain(result, last_result - 0.1, last_result + 0.1);

    result = min(result, FUEL_MAX_HZ);
    result = max(result, FUEL_MIN_HZ);

    if (prev_exhaust[2] - prev_exhaust[1] > 3 && prev_exhaust[1] - prev_exhaust[0] > 3 && prev_exhaust[0] - exhaust > 3 && result >= last_result)
    {
        Serial.println("Exhaust temperature decreasing during fuelling increase!");
        Serial.println(prev_exhaust[2]);
        Serial.println(prev_exhaust[1]);
        Serial.println(prev_exhaust[0]);
        exhaust_decreasing = true;
    }
    else if (prev_exhaust[2] - prev_exhaust[1] > 5 && prev_exhaust[1] - prev_exhaust[0] > 5 && prev_exhaust[0] - exhaust > 5)
    {
        Serial.println("Exhaust temperature decreasing too fast!");
        Serial.println(prev_exhaust[2]);
        Serial.println(prev_exhaust[1]);
        Serial.println(prev_exhaust[0]);
        exhaust_decreasing = true;
    }
    else if (prev_exhaust[1] - prev_exhaust[0] > 7 && prev_exhaust[0] - exhaust > 7) {
        exhaust_decreasing = true;
    }
    else
    {
        exhaust_decreasing = false;
    }

    prev_exhaust[2] = prev_exhaust[1];
    prev_exhaust[1] = prev_exhaust[0];
    prev_exhaust[0] = exhaust;

    last_result = result;

    return result;
}

#endif // INTELLIBASTO_BURNCTRL_H

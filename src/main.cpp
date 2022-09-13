#include <Arduino.h>
#include "config.h"
#include "TempCtrl.h"
#include "BurnCtrl.h"
#include "Matrix.h"
#include "Fuel.h"
#include "Glow.h"
#include "Modes.h"
#include "Fan.h"
#include "Pump.h"
#include "FuelMap.h"

TempCtrl temp;
BurnCtrl burnCtrl;
Modes modes;

Glow glow;
Fan fan;
Pump pump;
Fuel fuel;
Matrix matrix;

State state = IDLE;
long lastStateChange;
long lastLog;

int failures;

int exhaust_previous;

void setState(State s)
{
    state = s;
    lastStateChange = millis();
}

long timeInState()
{
    return millis() - lastStateChange;
}

void stateIdle()
{
    glow.set(0);
    fan.set(0);
    fuel.set(0);
    matrix.set(0);

    if (modes.get() != OFF && temp.water() > TEMP_SETPOINT - TEMP_BAND) {
        pump.set(255);
    } else {
        pump.set(0);
    }


    if (modes.get() != OFF && timeInState() > (long)MIN_TIME_IDLE)
    {
        setState(CIRCULATE);
        return;
    }

    if (modes.get() == OFF && (digitalRead(PIN_PRIME) == LOW))
    {
        setState(PRIME);
        return;
    }
}

void stateCirculate()
{
    pump.set(255);

    bool heatRequired = temp.water() < TEMP_SETPOINT - TEMP_BAND;

    if (modes.get() == OFF)
    {
        setState(IDLE);
        return;
    }

    if (timeInState() > (long)MIN_TIME_CIRCULATE)
    {
        if (heatRequired)
        {
            setState(PRE_IGNITION);
            return;
        }
        else
        {
            fan.set(0);
        }
    }
    else
    {
        fan.set(64);
    }
}

void statePreIgnition()
{
    glow.set(255);
    fan.set(32);
    pump.set(255);
    fuel.set(0);

    // last opportunity to turn back before we've got a fire on our hands.
    if (modes.get() == OFF)
    {
        setState(IDLE);
        return;
    }

    if (timeInState() > (long)TIME_PRE_IGNITION)
    {
        setState(IGNITION);
        exhaust_previous = temp.exhaust();
        return;
    }
}

void stateIgnition()
{
    glow.set(255);
    fan.set(32);
    pump.set(255);
    fuel.set(2);

    if (timeInState() > (long)TIME_IGNITION)
    {
        setState(POST_IGNITION);
        return;
    }
}

void statePostIgnition()
{
    glow.set(0);
    fan.set(32);
    pump.set(255);
    fuel.set(2); // run a bit rich whilst we warm up, gives best chance of successful burn.

    burnCtrl.reset();

    // todo, figure out if the heater has lit and if so commence to burn, otherwise abort!

    // if (temp.exhaust() > exhaust_previous + (EXHAUST_LIT_DELTA * 2))
    // {
    //     setState(BURN);
    //     exhaust_previous = temp.exhaust();
    // }

    if (timeInState() > (long)TIME_POST_IGNITION)
    {
        if (temp.exhaust() > exhaust_previous + EXHAUST_LIT_DELTA)
        {
            setState(BURN);
            exhaust_previous = temp.exhaust();
        }
        else
        {
            Serial.println("Exhaust temperature did not rise after ignition.");
            setState(COOL_DOWN);
        }
    }
}

void stateBurn()
{
    glow.set(0);
    pump.set(255);

    // if (modes.get() == HEAT_S2)
    // {
    //     fan.set(128);
    //     fuel.set(8);
    // }
    // else
    // {
    //     fan.set(48);
    //     fuel.set(2.5);
    // }

    double desired_fuelling_rate = burnCtrl.calculate(temp.water(), temp.exhaust());


    if (temp.exhaust() > 250) {
        desired_fuelling_rate = FUEL_MIN_HZ;
    }

    unsigned char desired_fan_speed = fuelmap(desired_fuelling_rate);

    // Serial.print(desired_fuelling_rate);
    // Serial.print(" ");
    // Serial.println(desired_fan_speed);

    if (temp.exhaust() < 100) {
        desired_fuelling_rate = desired_fuelling_rate * 1.25;
    }

    // exhaust temp drop is most likely the burner going out
    if (temp.exhaust() < 80 && timeInState() > TIME_BURN) {
        Serial.println("Insufficient exhaust temperature, possible burnout.");
        setState(COOL_DOWN);
    }

    if (timeInState() > TIME_BURN && !burnCtrl.lit()) {
        Serial.println("Exhaust temperature decreasing unexpectedly, possible burnout.");
        setState(COOL_DOWN);
    }

    if (timeInState() > (long)TIME_BURN && modes.get() == OFF)
    {
        Serial.println("Heat no longer requested, shutting off.");
        setState(COOL_DOWN);
    }

    if (temp.water() > TEMP_SETPOINT + TEMP_BAND) {
        Serial.println("Water at maximum temperature, shutting off.");
        setState(COOL_DOWN);
    }

    fan.set(desired_fan_speed);
    fuel.set(desired_fuelling_rate);

    // Exhaust temp watcher.
    // if the exhaust temp drops whilst the Hz on the heater remains the same, the flame has most likely gone out.
    // if (lastFuel == desired_fuelling_rate)


}

void stateCooldown()
{
    glow.set(0);
    fan.set(64);
    pump.set(255);
    fuel.set(0);

    if (timeInState() > (long)TIME_COOL_DOWN && temp.exhaust() < 50)
    {
        setState(IDLE);
        return;
    }
}

void stateFail()
{
    glow.set(0);
    fan.set(255);
    pump.set(255);
    fuel.set(0);
    matrix.set(255);

    if (
        timeInState() > (long)TIME_COOL_DOWN &&
        temp.exhaust() < 50 &&
        temp.water() < 50)
    {
        glow.set(0);
        fan.set(0);
        pump.set(0);
        fuel.set(0);
        matrix.set(0);

        if (timeInState() > (long)TIME_FAIL)
        {
            setState(IDLE);
            failures++;

            if (failures >= MAX_FAILS)
            {
                while (true)
                {
                    delay(5000);
                    Serial.println("Locked-out due to failure. Power cycle to reset.");
                }
            }
        }
    }
}

void statePrime()
{
    fuel.set(8);

    if (digitalRead(PIN_PRIME) == HIGH)
    {
        setState(IDLE);
        return;
    }
}

void sanity()
{
    if (temp.exhaust() > 300)
    {
        setState(COOL_DOWN);
    }

    if (temp.exhaust() > 350)
    {
        setState(FAIL);
        return;
    }

    if (temp.water() > TEMP_SETPOINT + (2 * TEMP_BAND))
    {
        setState(COOL_DOWN);
    }

    if (temp.water() > 85)
    {
        setState(FAIL);
        return;
    }
}

void setup()
{
    Serial.begin(9600);

    // Initialise the pins required.
    pinMode(PIN_MATRIX, OUTPUT);
    pinMode(PIN_FUEL, OUTPUT);
    pinMode(PIN_WATER, OUTPUT);
    pinMode(PIN_FAN, OUTPUT);
    pinMode(PIN_GLOW, OUTPUT);

    pinMode(PIN_RFH_1, INPUT_PULLUP);
    pinMode(PIN_RFH_2, INPUT_PULLUP);
    pinMode(PIN_PRIME, INPUT_PULLUP);
    // Don't need to set the pinMode for analogue pins.

    pinMode(LED_BUILTIN, OUTPUT);
    // Initialise any classes.

    glow.set(0);
    fan.set(0);
    pump.set(0);
    fuel.set(0);
    matrix.set(0);

    lastStateChange = millis();
    lastLog = millis();
    failures = 0;
}

void logging()
{
    if (millis() - lastLog < 1000)
        return;
    lastLog = millis();

    // print state
    switch (state)
    {
    case IDLE:
        Serial.print("IDLE");
        break;
    case CIRCULATE:
        Serial.print("CIRC");
        break;
    case PRE_IGNITION:
        Serial.print("PRIG");
        break;
    case IGNITION:
        Serial.print("IGNT");
        break;
    case POST_IGNITION:
        Serial.print("POIG");
        break;
    case BURN:
        Serial.print("BURN");
        break;
    case COOL_DOWN:
        Serial.print("COOL");
        break;
    case FAIL:
        Serial.print("FAIL");
        break;
    case PRIME:
        Serial.print("PRME");
        break;
    }

    Serial.print(" ");

    Serial.print(modes.get());
    Serial.print(" - W");
    Serial.print(temp.water());
    Serial.print(", E");
    Serial.print(temp.exhaust());
    Serial.print(" - F");
    Serial.print((float)fuel.get() / 32.0);
    Serial.print(", A");
    Serial.print(fan.get());
    Serial.print(", P");
    Serial.print(pump.get());
    Serial.print(", G");
    Serial.print(glow.get());
    Serial.print(", M");
    Serial.print(matrix.get());

    Serial.print(" - ");

    Serial.println(timeInState() / 1000);
}

void loop()
{
    Status err;
    err = temp.update();

    modes.update();
    glow.update();
    fan.update();
    pump.update();
    fuel.update();
    matrix.update();

    switch (state)
    {
    case IDLE:
        stateIdle();
        break;
    case CIRCULATE:
        stateCirculate();
        break;
    case PRE_IGNITION:
        statePreIgnition();
        break;
    case IGNITION:
        stateIgnition();
        break;
    case POST_IGNITION:
        statePostIgnition();
        break;
    case BURN:
        stateBurn();
        break;
    case COOL_DOWN:
        stateCooldown();
        break;
    case FAIL:
        stateFail();
        break;
    case PRIME:
        statePrime();
        break;
    }

    logging();
    sanity();

    if (err)
    {
        setState(FAIL);
        return;
    }
}
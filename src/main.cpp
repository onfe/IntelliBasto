#include <Arduino.h>
#include "config.h"
#include "BurnCtrl.h"
#include "Matrix.h"
#include "Fuel.h"
#include "Glow.h"
#include "Modes.hpp"
#include "Fan.h"
#include "Pump.h"
#include "FuelMap.h"
#include "Heater.hpp"
#include "State.hpp"
#include "state/Idle.hpp"
#include "state/Circulate.hpp"
#include "state/PreIgnition.hpp"

Heater heater;
State *currentState = new Idle();

unsigned long lastStateChange = 0;
unsigned long lastLog = 0;

void changeState(NextState s)
{
    // Tell the current state that' we're about to change to a new state, then delete it.
    currentState->post(heater);
    delete currentState; // todo: this method of changing state might turn the heap to swiss cheese so consider other options.

    switch (s) {
        case NextState::UNCHANGED:
            return;
        case NextState::IDLE:
            currentState = new Idle();
            break;
        case NextState::CIRCULATE:
            currentState = new Circulate();
            break;
        case NextState::PRE_IGNITION:
            currentState = new PreIgnition();
            break;
        default:
            currentState = new Idle();
    }

    // Let the new state set itself up.
    currentState->pre(heater);

    lastStateChange = millis();
}
//
//long timeInState()
//{
//    return millis() - lastStateChange;
//}

//
//void stateIgnition()
//{
//    glow.ramp(255);
//    // fan.set(32);
//    pump.on();
//    // fuel.set(2);
//
//    if (timeInState() > (long)TIME_IGNITION / 2) {
//        fan.ramp(48);
//        fuel.set(2.5);
//    } else {
//        fan.ramp(32);
//        if (timeInState() < 10000) {
//            fuel.set(1);
//        } else {
//            fuel.set(2);
//        }
//    }
//
//    if (timeInState() > (long)TIME_IGNITION)
//    {
//        setState(POST_IGNITION);
//        return;
//    }
//}
//
//void statePostIgnition()
//{
//    glow.ramp(0);
//    fan.ramp(32);
//    pump.on();
//    fuel.set(2.5); // run a bit rich whilst we warm up, gives best chance of successful burn.
//
//    burnCtrl.reset();
//
//    // todo, figure out if the heater has lit and if so commence to burn, otherwise abort!
//
//    // if (temp.exhaust() > exhaust_previous + (EXHAUST_LIT_DELTA * 2))
//    // {
//    //     setState(BURN);
//    //     exhaust_previous = temp.exhaust();
//    // }
//
//    if (temp.exhaust() > exhaust_previous + EXHAUST_LIT_DELTA && timeInState() > MIN_TIME_POST_IGNITION) {
//        setState(BURN);
//        exhaust_previous = temp.exhaust();
//    }
//
//
//    if (timeInState() > (long)MAX_TIME_POST_IGNITION)
//    {
//        if (temp.exhaust() > exhaust_previous + EXHAUST_LIT_DELTA)
//        {
//            setState(BURN);
//            exhaust_previous = temp.exhaust();
//        }
//        else
//        {
//            Serial.println("Exhaust temperature did not rise after ignition.");
//            setState(COOL_DOWN);
//        }
//    }
//}
//
//void stateBurn()
//{
//    glow.ramp(0);
//    pump.on();
//
//    // if (modes.get() == HEAT_S2)
//    // {
//    //     fan.set(128);
//    //     fuel.set(8);
//    // }
//    // else
//    // {
//    //     fan.set(48);
//    //     fuel.set(2.5);
//    // }
//
//    double desired_fuelling_rate = burnCtrl.calculate(temp.water(), temp.exhaust());
//
//
//    // if (temp.exhaust() > 250) {
//    //     desired_fuelling_rate = desired_fuelling_rate - 1;
//    // }
//
//    // if (temp.exhaust() > 255) {
//    //     desired_fuelling_rate = desired_fuelling_rate - 1;
//    // }
//
//    unsigned char desired_fan_speed = fuelmap(desired_fuelling_rate);
//
//    // Serial.print(desired_fuelling_rate);
//    // Serial.print(" ");
//    // Serial.println(desired_fan_speed);
//
//    if (temp.exhaust() < 120) {
//        desired_fuelling_rate = desired_fuelling_rate * 1.25;
//    }
//
//    // exhaust temp drop is most likely the burner going out
//    if (temp.exhaust() < 80 && timeInState() > TIME_BURN) {
//        Serial.println("Insufficient exhaust temperature, possible burnout.");
//        setState(COOL_DOWN);
//    }
//
//    if (timeInState() > TIME_BURN && !burnCtrl.lit()) {
//        Serial.println("Exhaust temperature decreasing unexpectedly, possible burnout.");
//        setState(COOL_DOWN);
//    }
//
//    if (timeInState() > (long)TIME_BURN && modes.get() == OFF)
//    {
//        Serial.println("Heat no longer requested, shutting off.");
//        setState(COOL_DOWN);
//    }
//
//    if (temp.water() > TEMP_SETPOINT + TEMP_BAND) {
//        Serial.println("Water at maximum temperature, shutting off.");
//        setState(COOL_DOWN);
//    }
//
//    fan.ramp(desired_fan_speed);
//    fuel.set(desired_fuelling_rate);
//
//    // Exhaust temp watcher.
//    // if the exhaust temp drops whilst the Hz on the heater remains the same, the flame has most likely gone out.
//    // if (lastFuel == desired_fuelling_rate)
//
//
//}
//
//void stateCooldown()
//{
//    glow.ramp(0);
//    fan.ramp(64);
//    pump.on();
//    fuel.set(0);
//
//    if (timeInState() > (long)TIME_COOL_DOWN && temp.exhaust() < 60)
//    {
//        setState(IDLE);
//        return;
//    }
//
//    if (timeInState() > (long)TIME_COOL_DOWN && modes.get() != OFF) {
//        setState(CIRCULATE);
//    }
//}
//
//void stateFail()
//{
//    glow.ramp(0);
//    fan.ramp(255);
//    pump.on();
//    fuel.set(0);
//    // matrix.set(64);
//
//    if (
//        timeInState() > (long)TIME_COOL_DOWN &&
//        temp.exhaust() < 50 &&
//        temp.water() < 50)
//    {
//        glow.ramp(0);
//        fan.ramp(0);
//        pump.off();
//        fuel.set(0);
//        // matrix.set(0);
//
//        if (timeInState() > (long)TIME_FAIL)
//        {
//            setState(IDLE);
//            failures++;
//
//            if (failures >= MAX_FAILS)
//            {
//                while (true)
//                {
//                    delay(5000);
//                    Serial.println("Locked-out due to failure. Power cycle to reset.");
//                }
//            }
//        }
//    }
//}
//
//void statePrime()
//{
//    fuel.set(8);
//
//    if (digitalRead(PIN_PRIME) == HIGH)
//    {
//        setState(IDLE);
//        return;
//    }
//}
//
//void handleMatrix() {
//    if (modes.get() == OFF || pump.get() < 255) {
//        matrix.ramp(0);
//        return;
//    }
//
//    if (modes.get() == HEAT_S2 && temp.water() > TEMP_SETPOINT - TEMP_BAND) {
//        matrix.ramp(255);
//        return;
//    }
//
//    if (modes.get() == HEAT_S2 && temp.water() > 45) {
//        matrix.ramp(255);
//        return;
//    }
//
//    if (modes.get() == HEAT_S1 && temp.water() > TEMP_SETPOINT + (TEMP_BAND / 2)) {
//        matrix.ramp(255);
//        return;
//    }
//
//    matrix.ramp(0);
//}
//
//void sanity()
//{
//    if (temp.exhaust() > 325)
//    {
//        setState(COOL_DOWN);
//    }
//
//    if (temp.exhaust() > 350)
//    {
//        setState(FAIL);
//        return;
//    }
//
//    if (temp.water() > TEMP_SETPOINT + (2 * TEMP_BAND))
//    {
//        setState(COOL_DOWN);
//    }
//
//    if (temp.water() > 85)
//    {
//        setState(FAIL);
//        return;
//    }
//}

void setup()
{
    Serial.begin(9600);

    // todo: fast PWM testing.
    // // Pins D9 and D10 - 62.5 kHz
    // TCCR1A = 0b00000001; // 8bit
    // TCCR1B = 0b00001001; // x1 fast pwm

    heater.modes.init();

    heater.matrix.init();
    heater.fuel.init();
    heater.pump.init();
    heater.fan.init();
    heater.glow.init();

    heater.water.init();
    heater.exhaust.init();

//    pinMode(LED_BUILTIN, OUTPUT); // todo

    lastStateChange = millis();
}

void logging()
{
    if (millis() - lastLog < 1000) return;
    lastLog = millis();

    // print state
    Serial.print(currentState->name);

    Serial.print(" ");

    Serial.print(heater.modes.getString());
    Serial.print(" - W");
    Serial.print(heater.water.temp());
    Serial.print(", E");
    Serial.print(heater.exhaust.temp());
    Serial.print(" - F");
    Serial.print((float)heater.fuel.get() / Fuel::multiplier);
    Serial.print(", A");
    Serial.print(heater.fan.get());
    Serial.print(", P");
    Serial.print(heater.pump.get());
    Serial.print(", G");
    Serial.print(heater.glow.get());
    Serial.print(", M");
    Serial.print(heater.matrix.get());

    Serial.print(" - ");


    unsigned long t = heater.timeInState / 1000;
    unsigned char sec = t%60;
    unsigned char min = (t/60)%60;
    unsigned int hrs =  (t/3600);

    if (hrs > 0) {
        if (hrs < 10) Serial.print("0");
        Serial.print(hrs);
        Serial.print(":");
    }

    if (min < 10) Serial.print("0");
    Serial.print(min);
    Serial.print(":");

    if (sec < 10) Serial.print("0");
    Serial.println(sec);
}

void loop()
{
    // Update 'heater' inputs & outputs.
    heater.modes.update();

    heater.matrix.update();
    heater.fuel.update();
    heater.pump.update();
    heater.fan.update();
    heater.glow.update();

    heater.water.update();
    heater.exhaust.update();

    heater.timeInState = millis() - lastStateChange;

    // run the current state.
    StateResult s = currentState->run(heater);


    if (s.error != Error::NONE) {
        Serial.print("Encountered an error in state ");
        Serial.println(currentState->name);

        heater.lastError = s.error;
    }

    // todo: handle resetting errors.

    if (s.nextState != NextState::UNCHANGED) {
        // let the current state know we're changing states.
        changeState(s.nextState);
    }

    // todo: sanity stuff like changing to failure if something really bad has happened.

//    sanity();
    logging();
}

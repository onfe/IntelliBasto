#include <Arduino.h>
#include "config.h"
#include "BurnCtrl.h"
#include "Matrix.h"
#include "Fuel.h"
#include "Glow.h"
#include "Modes.hpp"
#include "Fan.h"
#include "Pump.h"
#include "Heater.hpp"
#include "state/States.hpp"

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
        case NextState::IGNITION:
            currentState = new Ignition();
            break;
        case NextState::POST_IGNITION:
            currentState = new PostIgnition();
            break;
        case NextState::BURN:
            currentState = new Burn(); //todo
            break;
        case NextState::EXTINGUISH:
            currentState = new Extinguish();
            break;
        case NextState::COOL_DOWN:
            currentState = new CoolDown();
            break;
        case NextState::FAIL:
            currentState = new Failure();
            break;
    }

    // Let the new state set itself up.
    currentState->pre(heater);

    lastStateChange = millis();
}

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

void matrix() {
    if (heater.modes.get() == Mode::OFF) {
        heater.matrix.rampOff();
        return;
    }

    if (heater.pump.get() < 255) {
        heater.matrix.rampOff();
        return;
    }

    if (heater.lastError == Error::MINOR || heater.lastError == Error::MAJOR ) {
        heater.matrix.off();
    }

    // Handle Matrix
    if (heater.modes.get() == Mode::HEAT_S1) {
        if (heater.water.temp() > TEMP_SETPOINT + 1) {
            heater.matrix.rampOn();
        } else if (heater.water.temp() < TEMP_SETPOINT - 1) {
            heater.matrix.rampOff();
        }

    } else if (heater.modes.get() == Mode::HEAT_S2) {
        if (heater.water.temp() > 45) {
            heater.matrix.rampOn();
        } else if (heater.water.temp() < 40) {
            heater.matrix.rampOff();
        }
    } else {
        heater.matrix.off();
    }
}

void sanity() {
    // Prevent error de-escalation, force fuel off.
    if (heater.lastError == Error::MAJOR) {
        heater.fuel.off();
        return;
    }

    // Sensor error check (major)
    if (
        heater.exhaust.temp() > 500 ||
        heater.water.temp() > 120 ||
        heater.exhaust.temp() < -25 ||
        heater.water.temp() < -25
    ) {
        Serial.println("SANITY: SENSOR ERROR LIKELY!");
        changeState(NextState::FAIL);
        heater.lastError = Error::MAJOR;
        return;
    }

    // Exhaust temp FAR too hot (major)
    if (heater.exhaust.temp() > TEMP_EXHAUST_LIMIT)
    {
        Serial.println("SANITY: EXHAUST TOO HOT (MAJOR)");
        changeState(NextState::COOL_DOWN);
        heater.lastError = Error::MAJOR;
        return;
    }

    // Water temp FAR too hot (major)
    if (heater.water.temp() > 90)
    {
        Serial.println("SANITY: WATER TOO HOT (MAJOR)");
        changeState(NextState::COOL_DOWN);
        heater.lastError = Error::MAJOR;
        return;
    }

    // exhaust slightly too hot (minor)
    if (heater.exhaust.temp() > TEMP_EXHAUST_MAXIMUM + 20)
    {
        Serial.println("SANITY: EXHAUST TOO HOT (MINOR)");
        changeState(NextState::COOL_DOWN);
        heater.lastError = Error::MINOR;
    }

    // water slightly too hot (minor)
    if (heater.water.temp() > TEMP_UPPER + 10)
    {
        Serial.println("SANITY: WATER TOO HOT (MINOR)");
        changeState(NextState::COOL_DOWN);
        heater.lastError = Error::MINOR;
    }
}

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
    Serial.print(heater.water.temp(), 1);
    Serial.print(heater.water.increasing() ? "/" : heater.water.decreasing() ? "\\" : "~");
    Serial.print(heater.water.rateOfChange(), 1);
    Serial.print(", E");
    Serial.print(heater.exhaust.temp(), 1);
    Serial.print(heater.exhaust.increasing() ? "/" : heater.exhaust.decreasing() ? "\\" : "~");
    Serial.print(heater.exhaust.rateOfChange(), 1);
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
    Serial.print(sec);

    if (heater.lastError != Error::NONE){
        Serial.print(" ERR: ");
        switch (heater.lastError) {
            case Error::FAILED_TO_LIGHT:
                Serial.println("Failed to ignite.");
                break;
            case Error::MINOR:
                Serial.println("MINOR (will reset when call for heat disabled)");
                break;
            case Error::MAJOR:
                Serial.println("MAJOR (check logs & heater, power cycle to reset)");
                break;
            default:
                break;
        }
    } else {
        Serial.println(" OK");
    }
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

    if (s.error != Error::NONE && s.error != Error::RESET) {
        Serial.print("Encountered an error in state ");
        Serial.println(currentState->name);

        heater.lastError = s.error;
    }

    if (s.error == Error::RESET) {
        heater.lastError = Error::NONE;
    }

    // todo: handle resetting errors.

    if (s.nextState != NextState::UNCHANGED) {
        // let the current state know we're changing states.
        changeState(s.nextState);
    }

    matrix();
    sanity();
    logging();
}

//
// Created by Eddie on 29/12/2022.
//

#include "Output.hpp"

void Output::init() {
    pinMode(this->pin, OUTPUT);
    off();
}

void Output::off() {
    set(0);
}

void Output::on() {
    set(255);
}

unsigned char Output::get() {
    return current;
}

void Output::set(unsigned char value) {
    if (value == 0 && this->allowZero) {
        this->current = 0;
    } else {
        this->current = constrain(value, this->min, this->max);
    }

    analogWrite(this->pin, this->current);
}

void Output::update() {
    analogWrite(this->pin, this->current);
}

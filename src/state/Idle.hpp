#ifndef INTELLIBASTO_IDLE_HPP
#define INTELLIBASTO_IDLE_HPP

#include <Arduino.h>
#include "State.hpp"

class Idle : public State {
public:
    Idle() : State("Idle") {};
    void pre(Heater &h) override;
    StateResult run(Heater &h) override;
    void post(Heater &h) override;
};


#endif //INTELLIBASTO_IDLE_HPP

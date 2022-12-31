#ifndef INTELLIBASTO_PREIGNITION_HPP
#define INTELLIBASTO_PREIGNITION_HPP

#include <Arduino.h>
#include "State.hpp"

class PreIgnition : public State {
public:
    PreIgnition() : State("PrIg") {};
    void pre(Heater &h) override;
    StateResult run(Heater &h) override;
    void post(Heater &h) override;
};


#endif //INTELLIBASTO_PREIGNITION_HPP

#ifndef INTELLIBASTO_CIRCULATE_HPP
#define INTELLIBASTO_CIRCULATE_HPP

#include <Arduino.h>
#include "State.hpp"

class Circulate : public State {
public:
    Circulate() : State("Circ") {};
    void pre(Heater &h) override;
    StateResult run(Heater &h) override;
    void post(Heater &h) override;
};


#endif //INTELLIBASTO_CIRCULATE_HPP

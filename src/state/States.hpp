#ifndef INTELLIBASTO_STATES_HPP
#define INTELLIBASTO_STATES_HPP

#include <Arduino.h>
#include "State.hpp"

class Idle : public State {
public:
    Idle() : State("Idle") {};
    void pre(Heater &h) override;
    StateResult run(Heater &h) override;
    void post(Heater &h) override;
};

class Circulate : public State {
public:
    Circulate() : State("Circ") {};
    void pre(Heater &h) override;
    StateResult run(Heater &h) override;
    void post(Heater &h) override;
};

class PreIgnition : public State {
public:
    PreIgnition() : State("PrIg") {};
    void pre(Heater &h) override;
    StateResult run(Heater &h) override;
    void post(Heater &h) override;
};

class Ignition : public State {
public:
    Ignition() : State("Ignt") {};
    void pre(Heater &h) override;
    StateResult run(Heater &h) override;
    void post(Heater &h) override;
};

class PostIgnition : public State {
public:
    PostIgnition() : State("PoIg") {};
    void pre(Heater &h) override;
    StateResult run(Heater &h) override;
    void post(Heater &h) override;
};

class Burn : public State {
public:
    Burn() : State("Burn") {};
    void pre(Heater &h) override;
    StateResult run(Heater &h) override;
    void post(Heater &h) override;
};

class Extinguish : public State {
public:
    Extinguish() : State("Extg") {};
    void pre(Heater &h) override;
    StateResult run(Heater &h) override;
    void post(Heater &h) override;
};

class CoolDown : public State {
public:
    CoolDown() : State("Cool") {};
    void pre(Heater &h) override;
    StateResult run(Heater &h) override;
    void post(Heater &h) override;
};

class Failure : public State {
public:
    Failure() : State("Fail") {};
    void pre(Heater &h) override;
    StateResult run(Heater &h) override;
    void post(Heater &h) override;
};

#endif //INTELLIBASTO_STATES_HPP

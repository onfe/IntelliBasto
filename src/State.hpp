#ifndef INTELLIBASTO_STATE_HPP
#define INTELLIBASTO_STATE_HPP

#include "Heater.hpp"
#include "StateResult.hpp"

class State {
public:
    const char* name;
    explicit State(const char *name) : name(name) {}
    virtual ~State() = default;

    virtual void pre(Heater &h) = 0;
    virtual StateResult run(Heater &h) = 0;
    virtual void post(Heater &h) = 0;
};


#endif //INTELLIBASTO_STATE_HPP

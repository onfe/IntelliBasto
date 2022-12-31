//
// Created by Eddie on 29/12/2022.
//

#ifndef INTELLIBASTO_OUTPUT_HPP
#define INTELLIBASTO_OUTPUT_HPP

#include <Arduino.h>
#include "IO.hpp"


class Output : public IO {
protected:
    const unsigned char min;
    const unsigned char max;
    const bool allowZero;

private:
    unsigned char current = 0;

public:
    explicit Output(unsigned char pin) : IO{pin}, min{255}, max{255}, allowZero{true} {} // act as digital
    Output(unsigned char pin, unsigned char min, unsigned char max, bool allowZero) : IO{pin}, min{min}, max{max}, allowZero{allowZero} {}

    void init() override;
    void update() override;

    virtual void on();
    virtual void off();
    virtual unsigned char get();
    virtual void set(unsigned char value);
};


#endif //INTELLIBASTO_OUTPUT_HPP

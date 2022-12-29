#ifndef INTELLIBASTO_GLOW_H
#define INTELLIBASTO_GLOW_H

#include "config.h"
#include "SmoothedOutput.h"

class Glow : public SmoothedOutput {
    static const unsigned long maxOnTime = 2 * 60 * 1000;

    unsigned long timeLastOff = 0;
    bool on = false;

public:
    Glow() : SmoothedOutput(PIN_GLOW, 3000U, 0U, 255U, true) {}

    void update() override;
};

#endif //INTELLIBASTO_GLOW_H

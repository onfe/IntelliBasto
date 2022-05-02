#ifndef INTELLIBASTO_MODES_H
#define INTELLIBASTO_MODES_H

#include "types.h"


class Modes {
    static const unsigned char checkInterval = 10;

    unsigned char inputs[8] = {255};
    unsigned char index;

    unsigned long lastCheck;
    unsigned long lastModeChange;

    // set the heater to 'off' on startup.
    Mode currentMode = OFF;

public:
    Modes();

    void update();

    Mode get();
};


#endif //INTELLIBASTO_MODES_H

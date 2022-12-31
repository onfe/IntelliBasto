#ifndef INTELLIBASTO_MODES_HPP
#define INTELLIBASTO_MODES_HPP

#include "types.h"

class Modes {
    static const unsigned char checkInterval = 10;

    unsigned char inputs[8] = {255};
    unsigned char index;

    unsigned long lastCheck;
    unsigned long lastModeChange;

    // set the heater to 'off' on startup.
    Mode currentMode = Mode::OFF;

public:
    Modes();

    void update();
    void init();

    Mode get();

    const char * getString();
};


#endif //INTELLIBASTO_MODES_HPP

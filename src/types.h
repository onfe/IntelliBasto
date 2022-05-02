#ifndef INTELLIBASTO_TYPES_H
#define INTELLIBASTO_TYPES_H

enum Status {
    OK = 0,
    ERROR = 1
};

enum State {
    IDLE = 0,
    PRE_IGNITION = 1,
    IGNITION = 2,
    POST_IGNITION = 3,
    BURN = 4,
    COOL_DOWN = 5,
    FAIL = 6,
    PRIME = 7
};

enum Mode {
    OFF = 0,
    HEAT_S1 = 1,
    HEAT_S2 = 2
};

#endif //INTELLIBASTO_TYPES_H

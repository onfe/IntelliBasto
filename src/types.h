#ifndef INTELLIBASTO_TYPES_H
#define INTELLIBASTO_TYPES_H

enum Status {
    OK = 0,
    ERROR = 1
};

enum State {
    IDLE = 0,
    CIRCULATE = 1,
    PRE_IGNITION = 2,
    IGNITION = 3,
    POST_IGNITION = 4,
    BURN = 5,
    COOL_DOWN = 6,
    FAIL = 7,
    PRIME = 8
};

enum Mode {
    OFF = 0,
    HEAT_S1 = 1,
    HEAT_S2 = 2
};

#endif //INTELLIBASTO_TYPES_H

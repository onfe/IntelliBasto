#ifndef INTELLIBASTO_TYPES_H
#define INTELLIBASTO_TYPES_H

enum Status {
    OK = 0,
    ERROR = 1
};

enum class NextState {
    UNCHANGED = 0,
    IDLE = 1,
    CIRCULATE = 2,
    PRE_IGNITION = 3,
    IGNITION = 4,
    POST_IGNITION = 5,
    BURN = 6,
    COOL_DOWN = 7,
    FAIL = 8,
    PRIME = 9
};

enum class Error {
    NONE = 0,
    FAILED_TO_LIGHT = 1,
    MINOR = 2,
    MAJOR = 3
};

enum class Mode {
    OFF = 0,
    HEAT_S1 = 1,
    HEAT_S2 = 2,
    PRIME = 3
};

#endif //INTELLIBASTO_TYPES_H

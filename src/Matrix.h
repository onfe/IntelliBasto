#ifndef INTELLIBASTO_MATRIX_H
#define INTELLIBASTO_MATRIX_H

#include "config.h"
#include "SmoothedOutput.h"

#include <Arduino.h>

class Matrix : public SmoothedOutput {
public:
    Matrix() : SmoothedOutput(PIN_MATRIX, 5000U, 64U, 255U, true) {}
};

#endif //INTELLIBASTO_MATRIX_H

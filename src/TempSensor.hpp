#ifndef INTELLIBASTO_TEMPSENSOR_HPP
#define INTELLIBASTO_TEMPSENSOR_HPP

#include <Arduino.h>
#include "IO.hpp"

#define TS_SAMPLE_RATE 10 // ms
#define TS_TEMP_TIME 500 // ms
#define TS_HIST_TIME 10000 // ms

#define TS_MAX_DISCARDS 5

#define TS_TEMP_SIZE (TS_TEMP_TIME / TS_SAMPLE_RATE)
#define TS_HIST_SIZE ((TS_HIST_TIME / TS_TEMP_TIME) * 2)

class TempSensor : public IO {
    int temps[TS_TEMP_SIZE] {0};
    int history[TS_HIST_SIZE] {0};
    unsigned char tempIndex = 0;
    bool tempFilled = false;
    // unsigned char histIndex = 0;
    // bool histFilled = false;

    unsigned long lastUpdate {0};
    unsigned char discarded {0};

    const unsigned int pullup;
    const double sh_A;
    const double sh_B;
    const double sh_C;

    const float delta;

public:
    TempSensor(unsigned char pin, unsigned int pullup, double sh_A, double sh_B, double sh_C, float delta) : IO{pin}, pullup{pullup}, sh_A{sh_A}, sh_B{sh_B}, sh_C{sh_C}, delta{delta} {}

    void update() override;
    void init() override {}

    float temp();
    bool increasing();
    bool decreasing();
    double rateOfChange();
    bool stable();

private:

    float steinhart(double resistance) const;
    float calculate();
    static int average(const int arr[], int size);
};


#endif //INTELLIBASTO_TEMPSENSOR_HPP

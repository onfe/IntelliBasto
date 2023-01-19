#ifndef INTELLIBASTO_TEMPSENSOR_HPP
#define INTELLIBASTO_TEMPSENSOR_HPP

#include <Arduino.h>
#include "IO.hpp"

class TempSensor : public IO {
    constexpr static const int sample_rate = 100 / 1000;
    constexpr static const int temp_time = 500; // ms
    constexpr static const int history_time = 1000; // ms
    constexpr static const int history_samples = 50;


    int temps[temp_time / sample_rate] {};   // approx 1.5s averaging period at 20 samples / second.
    int history[64] {}; // approx 25s history, filled every time
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

    static const unsigned char delta = 1;

public:
    TempSensor(unsigned char pin, unsigned int pullup, double sh_A, double sh_B, double sh_C) : IO{pin}, pullup{pullup}, sh_A{sh_A}, sh_B{sh_B}, sh_C{sh_C} {}

    void update() override;
    void init() override {}

    float temp();
    bool increasing();
    bool decreasing();
    float rateOfChange();
    bool stable();

private:

    float steinhart(double resistance) const;
    float calculate();
    static int average(const int arr[], int size);
};


#endif //INTELLIBASTO_TEMPSENSOR_HPP

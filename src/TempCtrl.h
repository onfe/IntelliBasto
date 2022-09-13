#ifndef INTELLIBASTO_TEMPCTRL_H
#define INTELLIBASTO_TEMPCTRL_H

#include "config.h"
#include "types.h"

#include <stdint.h>
#include <Arduino.h>

class TempCtrl {
//    // sensor pins, exhaust is *always* index 0.
//    static constexpr unsigned short sensors[] = {
//        PIN_THERM_EXHAUST,
//        PIN_THERM_WATER_A,
//#ifdef USE_DUAL_WATER_TEMP
//        PIN_THERM_WATER_B
//#endif
//    };

    int waterTemps[64];
    int exhaustTemps[64];
    unsigned char index = 0;
    bool filled = false;
    unsigned long lastUpdate = millis();

public:
    TempCtrl();

    Status update();
    int water();
    int exhaust();

    private:
    int getTemp(int pin, bool exhaust);
    int steinhart(double r, double a, double b, double c);
    int computeAverage(int arr[], int size);
};


#endif //INTELLIBASTO_TEMPCTRL_H

//
// Created by Eddie on 30/04/2022.
//

#ifndef INTELLIBASTO_SMOOTHEDOUTPUT_H
#define INTELLIBASTO_SMOOTHEDOUTPUT_H


class SmoothedOutput {
protected:
    unsigned char pin;
    unsigned char min;
    unsigned char max;
    unsigned short smoothing;

    unsigned char from;
    unsigned char to;
    unsigned long startTime;
    bool allowZero;

public:

    SmoothedOutput(unsigned char pin, unsigned short smoothing, unsigned char min = 0, unsigned char max = 255, bool allowZero = true);

    virtual void update();

    virtual void set(unsigned char value);

    virtual unsigned char get();
};


#endif //INTELLIBASTO_SMOOTHEDOUTPUT_H

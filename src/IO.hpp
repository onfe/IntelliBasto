//
// Created by Eddie on 29/12/2022.
//

#ifndef INTELLIBASTO_IO_HPP
#define INTELLIBASTO_IO_HPP


class IO {
protected:
    const unsigned char pin;
    explicit IO(unsigned char pin) : pin(pin) {}

    public:
    virtual void update() = 0;
    virtual void init() = 0;
};


#endif //INTELLIBASTO_IO_HPP

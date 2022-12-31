#ifndef INTELLIBASTO_STATERESULT_HPP
#define INTELLIBASTO_STATERESULT_HPP

struct StateResult {
    const Error error;
    const NextState nextState;

    explicit StateResult(Error error, NextState nextState = NextState::UNCHANGED) : error(error), nextState(nextState) {}
};

#endif //INTELLIBASTO_STATERESULT_HPP

//
// Created by S.Morgenthaler on 24/12/2022.
//

#include "DoubleChronometer.h"

DoubleChronometer::DoubleChronometer(
    bool firstAutoStart,
    bool secondAutoStart
) :
    _chronometers{
        std::make_shared<Chronometer>(firstAutoStart),
        std::make_shared<Chronometer>(secondAutoStart),
    },
    _first(_chronometers.front()),
    _second(_chronometers.back()),
    _cstFirst(_first),
    _cstSecond(_second) {
}

void DoubleChronometer::update(const Chronometer::TimePointMs& updatingTime) {
    for (auto &chronometer: _chronometers) {
        chronometer->update(updatingTime);
    }
}

void DoubleChronometer::reset() {
    for (auto &chronometer: _chronometers) {
        chronometer->reset();
    }
}

void DoubleChronometer::stopFirst() {
    first()->stop();
}

void DoubleChronometer::stopSecond() {
    second()->stop();
}

void DoubleChronometer::stopBoth() {
    for (auto &chronometer: _chronometers) {
        chronometer->stop();
    }
}

void DoubleChronometer::resumeFirst() {
    first()->resume();
}

void DoubleChronometer::resumeSecond() {
    second()->resume();
}

void DoubleChronometer::resumeBoth() {
    for (auto &chronometer: _chronometers) {
        chronometer->resume();
    }
}

float DoubleChronometer::getFirstTimer() const {
    return first()->getTime();
}

float DoubleChronometer::getSecondTimer() const {
    return second()->getTime();
}

const CstChronometer_sptr &DoubleChronometer::first() const {
    return _cstFirst;
}

const CstChronometer_sptr &DoubleChronometer::second() const {
    return _cstSecond;
}

const Chronometer_sptr &DoubleChronometer::first() {
    return _first;
}

const Chronometer_sptr &DoubleChronometer::second() {
    return _second;
}

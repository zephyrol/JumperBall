//
// Created by S.Morgenthaler on 24/12/2022.
//

#include "DoubleChronometer.h"

DoubleChronometer::DoubleChronometer() : _chronometers() {

}

void DoubleChronometer::update() {
    for (auto &chronometer: _chronometers) {
        chronometer.update();
    }
}

void DoubleChronometer::reset() {
    for (auto &chronometer: _chronometers) {
        chronometer.reset();
    }
}

void DoubleChronometer::stopFirst() {
    getFirst().stop();
}

void DoubleChronometer::stopSecond() {
    getSecond().stop();
}

void DoubleChronometer::stopBoth() {
    for (auto &chronometer: _chronometers) {
        chronometer.stop();
    }
}

void DoubleChronometer::resumeFirst() {
    getFirst().resume();
}

void DoubleChronometer::resumeSecond() {
    getSecond().resume();
}

void DoubleChronometer::resumeBoth() {
    for (auto &chronometer: _chronometers) {
        chronometer.resume();
    }
}

float DoubleChronometer::getFirstTimer() const {
    return getFirst().getTime();
}

float DoubleChronometer::getSecondTimer() const {
    return getSecond().getTime();
}

const Chronometer &DoubleChronometer::getFirst() const {
    return _chronometers.front();
}

const Chronometer &DoubleChronometer::getSecond() const {
    return _chronometers.back();
}

const DoubleChronometer &DoubleChronometer::getConstThis() {
    return static_cast <const DoubleChronometer &>(*this);
}

Chronometer &DoubleChronometer::getFirst() {
    // Scott Meyer's advice to avoid code duplication
    return const_cast <Chronometer &>(getConstThis().getFirst());
}

Chronometer &DoubleChronometer::getSecond() {
    // Scott Meyer's advice to avoid code duplication
    return const_cast <Chronometer &>(getConstThis().getSecond());
}

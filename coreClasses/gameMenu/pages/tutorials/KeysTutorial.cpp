//
// Created by S.Morgenthaler on 28/03/2024.
//

#include "KeysTutorial.h"

KeysTutorial::KeysTutorial(CstMovableObject_sptr movableObject, bool isInEnglish)
    : _movableObject(std::move(movableObject)),
      _isInEnglish(isInEnglish),
      _currentStep(0),
      _chronometer(true),
      _stepsConditions({[this]() { return _chronometer.getTime() > 2.f; },
                        [this]() { return _chronometer.getTime() > 4.f; },
                        [this]() { return _chronometer.getTime() > 2.f; },
                        [this]() { return _movableObject->numberOfKeys() == 2; },
                        [this]() { return _chronometer.getTime() > 2.f; },
                        [this]() { return _chronometer.getTime() > 8.f; }, [this]() { return false; }}),
      _numberIdTable({-1, 0, 0, 1, 1, 2, 2, 3, 3}) {}

std::vector<Tutorial::Message> KeysTutorial::getMessages() const {
    if (_isInEnglish) {
        return {
            {"Watch out for spikes!", ""}, {"Get all keys to", "unlock the exit"}, {"The exit is open!", ""}};
    }
    return {{"Attention aux pics !", ""},
            {"Trouve toutes les cl#s", "pour ouvrir la sortie"},
            {"La sortie est ouverte !", ""}};
}

float KeysTutorial::getAnimationTime() {
    return _currentStep % 2 == 1 ? _chronometer.getTime() : -std::min(1.f, _chronometer.getTime());
}

short KeysTutorial::getCurrentMessageNumberId() const {
    return _numberIdTable[_currentStep];
}

void KeysTutorial::update() {
    _chronometer.update(Chronometer::getTimePointMSNow());
    if (_stepsConditions[_currentStep]()) {
        ++_currentStep;
        _chronometer.reset();
    }
}

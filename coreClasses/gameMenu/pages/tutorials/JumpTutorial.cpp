//
// Created by S.Morgenthaler on 28/03/2024.
//

#include "JumpTutorial.h"

JumpTutorial::JumpTutorial(CstMovableObject_sptr movableObject, bool isInEnglish)
    : _movableObject(std::move(movableObject)),
      _isInEnglish(isInEnglish),
      _currentStep(0),
      _chronometer(true),
      _stepsConditions({[this]() { return _chronometer.getTime() > 2.f; },
                        [this]() { return _movableObject->isJumping(); }, [this]() { return false; }}),
      _numberIdTable({-1, 0, 0}) {}

std::vector<Tutorial::Message> JumpTutorial::getMessages() const {
    return _isInEnglish ? std::vector<Tutorial::Message>{{"Tap on screen to jump!", ""}}
                        : std::vector<Tutorial::Message>{{"Tapez sur l';cran pour sauter!", ""}};
}

float JumpTutorial::getAnimationTime() {
    return _currentStep % 2 == 1 ? _chronometer.getTime() : -std::min(1.f, _chronometer.getTime());
}

short JumpTutorial::getCurrentMessageNumberId() const {
    return _numberIdTable[_currentStep];
}

void JumpTutorial::update() {
    _chronometer.update(Chronometer::getTimePointMSNow());
    if (_stepsConditions[_currentStep]()) {
        ++_currentStep;
        _chronometer.reset();
    }
}

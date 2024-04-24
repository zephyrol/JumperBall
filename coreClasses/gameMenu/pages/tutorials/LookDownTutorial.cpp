//
// Created by S.Morgenthaler on 28/03/2024.
//

#include "LookDownTutorial.h"

LookDownTutorial::LookDownTutorial(CstMovableObject_sptr movableObject,
                                   CstAboveMovingCamera_sptr camera,
                                   bool isInEnglish,
                                   bool isUsingTouchScreen)
    : _movableObject(std::move(movableObject)),
      _isInEnglish(isInEnglish),
      _isUsingTouchScreen(isUsingTouchScreen),
      _currentStep(0),
      _camera(std::move(camera)),
      _chronometer(true),
      _stepsConditions({[this]() { return _chronometer.getTime() > 2.f; },
                        [this]() { return _camera->isMovingAbove(); }, [this]() { return false; }}),
      _numberIdTable({-1, 0, 0}) {}

std::vector<Tutorial::Message> LookDownTutorial::getMessages() const {
    if (_isUsingTouchScreen) {
        return _isInEnglish ? std::vector<Tutorial::Message>{{"Swipe down to", "look down"}}
                            : std::vector<Tutorial::Message>{{"Balayez vers le bas", "pour regarder en bas"}};
    }
    return _isInEnglish
               ? std::vector<Tutorial::Message>{{"Press the down arrow", "to look down"}}
               : std::vector<Tutorial::Message>{{"Appuyez sur la fl^che du", "bas pour regarder en bas"}};
}

float LookDownTutorial::getAnimationTime() {
    return _currentStep % 2 == 1 ? _chronometer.getTime() : -std::min(1.f, _chronometer.getTime());
}

short LookDownTutorial::getCurrentMessageNumberId() const {
    return _numberIdTable[_currentStep];
}

void LookDownTutorial::update() {
    _chronometer.update(Chronometer::getTimePointMSNow());
    if (_stepsConditions[_currentStep]()) {
        ++_currentStep;
        _chronometer.reset();
    }
}

//
// Created by S.Morgenthaler on 28/03/2024.
//

#include "LookDownTutorial.h"

LookDownTutorial::LookDownTutorial(CstMovableObject_sptr movableObject,
                                   CstAboveMovingCamera_sptr camera,
                                   bool isInEnglish)
    : _movableObject(std::move(movableObject)),
      _isInEnglish(isInEnglish),
      _currentStep(0),
      _camera(std::move(camera)),
      _chronometer(true),
      _stepsConditions({[this]() { return _chronometer.getTime() > 2.f; },
                        [this]() { return _camera->isMovingAbove(); }, [this]() { return false; }}),
      _numberIdTable({-1, 0, 0}) {}

std::vector<Tutorial::Message> LookDownTutorial::getMessages() const {
    return _isInEnglish ? std::vector<Tutorial::Message>{{"You can move on all", "sides of the maze!"}}
                        : std::vector<Tutorial::Message>{
                              {"Vous pouvez avancer sur toutes", "les lookDown du labyrinthe !"}};
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

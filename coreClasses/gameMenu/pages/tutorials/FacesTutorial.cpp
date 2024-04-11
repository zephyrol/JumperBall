//
// Created by S.Morgenthaler on 28/03/2024.
//

#include "FacesTutorial.h"

FacesTutorial::FacesTutorial(CstMovableObject_sptr movableObject, bool isInEnglish)
    : _movableObject(std::move(movableObject)),
      _isInEnglish(isInEnglish),
      _currentStep(0),
      _chronometer(true),
      _stepsConditions({[this]() { return _chronometer.getTime() > 2.f; },
                        [this]() { return _movableObject->hasChangedAFace(); }, [this]() { return false; }}),
      _numberIdTable({-1, 0, 0}) {}

std::vector<Tutorial::Message> FacesTutorial::getMessages() const {
    return _isInEnglish ? std::vector<Tutorial::Message>{{"You can move on all", "sides of the maze!" }}
                        : std::vector<Tutorial::Message>{{"Vous pouvez avancer sur toutes", "les faces du labyrinthe !"}};
}

float FacesTutorial::getAnimationTime() {
    return _currentStep % 2 == 1 ? _chronometer.getTime() : -std::min(1.f, _chronometer.getTime());
}

short FacesTutorial::getCurrentMessageNumberId() const {
    return _numberIdTable[_currentStep];
}

void FacesTutorial::update() {
    _chronometer.update(Chronometer::getTimePointMSNow());
    if (_stepsConditions[_currentStep]()) {
        ++_currentStep;
        _chronometer.reset();
    }
}

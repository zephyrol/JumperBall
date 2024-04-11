//
// Created by S.Morgenthaler on 28/03/2024.
//

#include "MovementTutorial.h"

MovementTutorial::MovementTutorial(CstMovableObject_sptr movableObject,
                                   bool isInEnglish,
                                   bool isUsingTouchScreen)
    : _movableObject(std::move(movableObject)),
      _isInEnglish(isInEnglish),
      _isUsingTouchScreen(isUsingTouchScreen),
      _currentStep(0),
      _chronometer(true),
      _stepsConditions({[this]() { return _chronometer.getTime() > 2.f; },
                        [this]() { return _movableObject->isGoingAhead(); },
                        [this]() { return _chronometer.getTime() > 2.f; },
                        [this]() { return _movableObject->isTurningRight(); },
                        [this]() { return _chronometer.getTime() > 2.f; },
                        [this]() { return _movableObject->isTurningLeft(); },
                        [this]() { return _chronometer.getTime() > 2.f; },
                        [this]() { return _chronometer.getTime() > 8.f; }, [this]() { return false; }}),
      _numberIdTable({-1, 0, 0, 1, 1, 2, 2, 3, 3}) {}

std::vector<Tutorial::Message> MovementTutorial::getMessages() const {
    const Tutorial::Message finishLevel =
        _isInEnglish ? Tutorial::Message{"Go to the green circle", "to finish the level"}
                     : Tutorial::Message{"Allez sur le cercle vert", "pour finir le niveau"};

    if (_isUsingTouchScreen) {
        if (_isInEnglish) {
            return {{"Swipe up to go ahead", ""},
                    {"Swipe left to turn right", ""},
                    {"Swipe right to turn left", ""},
                    finishLevel};
        }
        return {{"Balayez vers le haut", "pour avancer"},
                {"Balayez vers la gauche", "pour tourner à droite"},
                {"Balayez vers la droite", "pour tourner à gauche"},
                finishLevel};
    }
    if (_isInEnglish) {
        return {{"Press the up arrow", "to move forward"},
                {"Press the right arrow", "to turn right"},
                {"Press the left arrow", "to turn left"},
                finishLevel};
    }
    return {{"Appuyez sur la fl^che", "du haut pour avancer"},
            {"Appuyez sur la fl^che", "de droite pour tourner à droite"},
            {"Appuyez sur la fl^che", "de gauche pour tourner à gauche"},
            finishLevel};
}

float MovementTutorial::getAnimationTime() {
    return _currentStep % 2 == 1 ? _chronometer.getTime() : -std::min(1.f, _chronometer.getTime());
}

short MovementTutorial::getCurrentMessageNumberId() const {
    return _numberIdTable[_currentStep];
}

void MovementTutorial::update() {
    _chronometer.update(Chronometer::getTimePointMSNow());
    if (_stepsConditions[_currentStep]()) {
        ++_currentStep;
        _chronometer.reset();
    }
}

//
// Created by S.Morgenthaler on 28/03/2024.
//

#include "MovementTutorial.h"

MovementTutorial::MovementTutorial(CstMovableObject_sptr movableObject,
                                   bool isInEnglish,
                                   bool isUsingTouchScreen,
                                   bool isLeftRightReverted)
    : _movableObject(std::move(movableObject)),
      _isInEnglish(isInEnglish),
      _isUsingTouchScreen(isUsingTouchScreen),
      _isLeftRightReverted(isLeftRightReverted),
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
                    {"Swipe " + std::string(_isLeftRightReverted ? "left" : "right") + " to turn right", ""},
                    {"Swipe " + std::string(_isLeftRightReverted ? "right" : "left") + " to turn left", ""},
                    finishLevel};
        }
        return {{"Balayez vers le haut", "pour avancer"},
                {"Balayez vers la " + std::string(_isLeftRightReverted ? "gauche" : "droite"),
                 "pour tourner ` droite"},
                {"Balayez vers la " + std::string(_isLeftRightReverted ? "droite" : "gauche"),
                 "pour tourner ` gauche"},
                finishLevel};
    }
    if (_isInEnglish) {
        return {{"Press the up arrow", "to move forward"},
                {"Press the right arrow", "to turn right"},
                {"Press the left arrow", "to turn left"},
                finishLevel};
    }
    return {{"Appuyez sur la fl^che", "du haut pour avancer"},
            {"Appuyez sur la fl^che", "de droite pour tourner ` droite"},
            {"Appuyez sur la fl^che", "de gauche pour tourner ` gauche"},
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

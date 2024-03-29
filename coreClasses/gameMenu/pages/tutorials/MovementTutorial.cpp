//
// Created by S.Morgenthaler on 28/03/2024.
//

#include "MovementTutorial.h"

MovementTutorial::MovementTutorial(CstMovableObject_sptr movableObject, bool isInEnglish):
    _movableObject(std::move(movableObject)),
    _isInEnglish(isInEnglish),
    _currentStep(0),
    _chronometer(true) {
}

std::vector<Tutorial::Message> MovementTutorial::getMessages() const {
    if (_isInEnglish) {
        return {
            {"Swipe up to go ahead", ""},
            {"Swipe left to turn right", ""},
            {"Swipe right to turn left", ""},
            {"Go to the green circle", "to finish the level"}
        };
    }
    return {
        {"Balayez vers le haut", "pour avancer"},
        {"Balayez vers la gauche", "pour tourner à droite"},
        {"Balayez vers la droite", "pour tourner à gauche"},
        {"Allez sur le cercle vert", "pour finir le niveau"}
    };
}

float MovementTutorial::getAnimationTime() {
    if (_currentStep < 2) {
        return _chronometer.getTime();
    }
    return -std::min(1.f, _chronometer.getTime());
}

short MovementTutorial::getCurrentMessageNumberId() const {
    if(_currentStep == 0) {
        return -1;
    }
    if(_currentStep < 3) {
        return 0;
    }
    return -1;
}

void MovementTutorial::update() {
    _chronometer.update(Chronometer::getTimePointMSNow());
    if(_currentStep == 0) {
        if (_chronometer.getTime() > 2.f) {
            _currentStep++;
            _chronometer.reset();
        }
        return;
    }
    if(_currentStep == 1) {
        if (_movableObject->isGoingAhead()) {
            _chronometer.reset();
            _currentStep++;
        }
        return;
    }
    if(_currentStep == 2) {
        if (_chronometer.getTime() > 3.f) {
            _chronometer.reset();
            _currentStep++;
        }
        return;
    }
}

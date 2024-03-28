//
// Created by S.Morgenthaler on 28/03/2024.
//

#include "MovementTutorial.h"

MovementTutorial::MovementTutorial(CstMovableObject_sptr movableObject, bool isInEnglish):
    _movableObject(std::move(movableObject)),
    _isInEnglish(isInEnglish) {
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

short MovementTutorial::getCurrentMessageNumberId() const {
    return 0;
}

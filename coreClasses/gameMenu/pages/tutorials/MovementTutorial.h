//
// Created by S.Morgenthaler on 28/03/2024.
//

#ifndef MOVEMENTTUTORIAL_H
#define MOVEMENTTUTORIAL_H
#include "Tutorial.h"
#include "system/Chronometer.h"

class MovementTutorial : public Tutorial {
public:
    explicit MovementTutorial(CstMovableObject_sptr movableObject, bool isInEnglish, bool isUsingTouchScreen);

    std::vector<Tutorial::Message> getMessages() const override;

    float getAnimationTime() override;

    short getCurrentMessageNumberId() const override;

    void update() override;

private:
    const CstMovableObject_sptr _movableObject;
    const bool _isInEnglish;
    const bool _isUsingTouchScreen;
    unsigned int _currentStep;
    Chronometer _chronometer;
};


#endif //MOVEMENTTUTORIAL_H

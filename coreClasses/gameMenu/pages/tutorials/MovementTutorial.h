//
// Created by S.Morgenthaler on 28/03/2024.
//

#ifndef MOVEMENTTUTORIAL_H
#define MOVEMENTTUTORIAL_H
#include "Tutorial.h"
#include "scene/MovableObject.h"
#include "system/Chronometer.h"

class MovementTutorial : public Tutorial {
   public:
    explicit MovementTutorial(CstMovableObject_sptr movableObject, bool isInEnglish, bool isUsingTouchScreen, bool isLeftRightReverted);

    std::vector<Tutorial::Message> getMessages() const override;

    float getAnimationTime() override;

    short getCurrentMessageNumberId() const override;

    void update() override;

   private:
    const CstMovableObject_sptr _movableObject;
    const bool _isInEnglish;
    const bool _isUsingTouchScreen;
    const bool _isLeftRightReverted;
    size_t _currentStep;
    Chronometer _chronometer;
    const std::vector<std::function<bool()>> _stepsConditions;
    const std::vector<short> _numberIdTable;
};

#endif  // MOVEMENTTUTORIAL_H

//
// Created by S.Morgenthaler on 08/04/2024.
//

#ifndef LOOKDOWNTUTORIAL_H
#define LOOKDOWNTUTORIAL_H
#include "Tutorial.h"
#include "scene/AboveMovingCamera.h"
#include "scene/MovableObject.h"
#include "system/Chronometer.h"

class LookDownTutorial : public Tutorial {
   public:
    explicit LookDownTutorial(CstMovableObject_sptr movableObject, CstAboveMovingCamera_sptr camera, bool isInEnglish);

    std::vector<Tutorial::Message> getMessages() const override;

    float getAnimationTime() override;

    short getCurrentMessageNumberId() const override;

    void update() override;

   private:
    const CstMovableObject_sptr _movableObject;
    const CstAboveMovingCamera_sptr _camera;
    const bool _isInEnglish;
    size_t _currentStep;
    Chronometer _chronometer;
    const std::vector<std::function<bool()>> _stepsConditions;
    const std::vector<short> _numberIdTable;
};

#endif  // LOOKDOWNTUTORIAL_H

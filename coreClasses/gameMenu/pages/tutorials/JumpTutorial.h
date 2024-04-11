//
// Created by S.Morgenthaler on 08/04/2024.
//

#ifndef JUMPTUTORIAL_H
#define JUMPTUTORIAL_H
#include "Tutorial.h"
#include "scene/MovableObject.h"
#include "system/Chronometer.h"

class JumpTutorial : public Tutorial {
   public:
    explicit JumpTutorial(CstMovableObject_sptr movableObject, bool isInEnglish);

    std::vector<Tutorial::Message> getMessages() const override;

    float getAnimationTime() override;

    short getCurrentMessageNumberId() const override;

    void update() override;

   private:
    const CstMovableObject_sptr _movableObject;
    const bool _isInEnglish;
    size_t _currentStep;
    Chronometer _chronometer;
    const std::vector<std::function<bool()>> _stepsConditions;
    const std::vector<short> _numberIdTable;
};

#endif  // JUMPTUTORIAL_H

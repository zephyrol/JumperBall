//
// Created by S.Morgenthaler on 08/04/2024.
//

#ifndef KEYSTUTORIAL_H
#define KEYSTUTORIAL_H
#include "Tutorial.h"
#include "scene/MovableObject.h"
#include "system/Chronometer.h"

class KeysTutorial : public Tutorial {
   public:
    explicit KeysTutorial(CstMovableObject_sptr movableObject, bool isInEnglish);

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

#endif  // KEYSTUTORIAL_H

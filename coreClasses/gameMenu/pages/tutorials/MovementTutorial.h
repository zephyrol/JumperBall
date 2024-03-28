//
// Created by S.Morgenthaler on 28/03/2024.
//

#ifndef MOVEMENTTUTORIAL_H
#define MOVEMENTTUTORIAL_H
#include "Tutorial.h"

class MovementTutorial : public Tutorial {
public:
    explicit MovementTutorial(CstMovableObject_sptr movableObject, bool isInEnglish);

    std::vector<Tutorial::Message> getMessages() const override;

    short getCurrentMessageNumberId() const override;

private:
    const CstMovableObject_sptr _movableObject;
    const bool _isInEnglish;
};


#endif //MOVEMENTTUTORIAL_H

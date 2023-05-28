//
// Created by S.Morgenthaler on 23/10/2021.
//

#ifndef JUMPERBALLAPPLICATION_INTERACTIVESPECIAL_H
#define JUMPERBALLAPPLICATION_INTERACTIVESPECIAL_H
#include <scene/Ball.h>
#include "Special.h"

class InteractiveSpecial: public Special{
public:
    InteractiveSpecial(
        const JBTypes::Color& color,
        const JBTypes::vec3ui& position,
        const JBTypes::Dir& direction,
        const Ball_sptr &ball,
        bool isActivated = true
        );

protected:
    const Ball_wptr _ball;

};


#endif //JUMPERBALLAPPLICATION_INTERACTIVESPECIAL_H

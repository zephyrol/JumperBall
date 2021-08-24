//
// Created by sebastien on 24/08/2021.
//

#ifndef JUMPERBALLAPPLICATION_INTERACTIVEENEMY_H
#define JUMPERBALLAPPLICATION_INTERACTIVEENEMY_H


#include <scene/Ball.h>
#include "Enemy.h"

class InteractiveEnemy: public Enemy{
public:
    InteractiveEnemy(
        const JBTypes::vec3ui& initialPosition,
        const JBTypes::Dir& direction,
        float size,
        size_t length,
        const std::array <float, 9>& transform,
        const Ball_sptr &ball
    );

protected:
    const Ball_sptr _ball;
};


#endif //JUMPERBALLAPPLICATION_INTERACTIVEENEMY_H

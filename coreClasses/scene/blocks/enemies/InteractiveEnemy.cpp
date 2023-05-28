//
// Created by S.Morgenthaler on 24/08/2021.
//

#include "InteractiveEnemy.h"

InteractiveEnemy::InteractiveEnemy(
    const JBTypes::vec3ui &initialPosition,
    const JBTypes::Dir &direction,
    float size,
    size_t length,
    const Ball_sptr &ball,
    bool isActivated
) : Enemy(initialPosition, direction, size, length, isActivated),
    _ball(ball) {
}

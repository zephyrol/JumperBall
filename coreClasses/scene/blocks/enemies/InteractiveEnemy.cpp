//
// Created by sebastien on 24/08/2021.
//

#include "InteractiveEnemy.h"

InteractiveEnemy::InteractiveEnemy(
    const JBTypes::vec3ui &initialPosition,
    const JBTypes::Dir &direction,
    float size,
    size_t length,
    const std::array<float, 9> &transform,
    const Ball_sptr &ball
):
Enemy(initialPosition, direction, size, length, transform),
_ball(ball) {
}

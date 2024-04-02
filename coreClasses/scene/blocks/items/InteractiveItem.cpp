//
// Created by S.Morgenthaler on 16/01/2022.
//

#include "InteractiveItem.h"

InteractiveItem::InteractiveItem(
    const glm::u32vec3 &position,
    const JBTypes::Dir &direction,
    const Ball_sptr &ball
) : Item(position, direction, ball->getCreationChronometer()),
    _ball(ball) {
}

void InteractiveItem::catchingTest() {
    if (isGotten()) {
        return;
    }
    const auto ball = _ball.lock();
    const auto &ballBoundingSpherePosition = ball->get3DPosition();
    const auto &ballBoundingSphereRadius = ball->getRadius();
    const float distance = JBTypesMethods::distance(get3DPosition(), ballBoundingSpherePosition);
    const auto& state = _ball.lock()->state();
    const auto itemBoundingSphereRadius = state == Ball::State::Jumping || state == Ball::State::Falling
        ? Item::itemBoundingSphereRadiusJumping
        : Item::itemBoundingSphereRadiusStaying;
    if (distance < ballBoundingSphereRadius + itemBoundingSphereRadius) {
        setAsGotten();
        catchItem();
    }
}

void InteractiveItem::catchItem() {
}

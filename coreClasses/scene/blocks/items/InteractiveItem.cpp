//
// Created by S.Morgenthaler on 16/01/2022.
//

#include "InteractiveItem.h"

InteractiveItem::InteractiveItem(
    const JBTypes::vec3ui &position,
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
    const auto &boundingSpherePosition = ball->get3DPosition();
    const auto &boundingSphereRadius = ball->getRadius();
    const float distance = JBTypesMethods::distance(get3DPosition(), boundingSpherePosition);
    if (distance < boundingSphereRadius + itemBoundingSphereRadius) {
        setAsGotten();
        catchItem();
    }
}

void InteractiveItem::catchItem() {
}

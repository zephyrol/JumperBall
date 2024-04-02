/*
 * File: Coin.cpp
 * Author: Morgenthaler S
 *
 * Created on 27 mars 2020, 20:49
 */

#include "Coin.h"

Coin::Coin(
    const glm::u32vec3 &position,
    const JBTypes::Dir &direction,
    const Ball_sptr &ball
) :
    Bonus(position, direction, 100, ball) {
}

vecCstShape_sptr Coin::getShapes() const {

    const auto coinShape = std::make_shared<const Shape>(
        Shape::Aspect::Cylinder,
        JBTypes::Color::Orange,
        std::initializer_list<Transformation>(
            {
                Transformation(Transformation::Type::Translation,
                               {0.f, -0.5f, 0.f}),
                Transformation(Transformation::Type::Scale,
                               {0.3f, 0.05f, 0.3f}),
                Item::getVerticalCylinderRotation(direction())
            }
        )
    );

    return {coinShape};
}

void Coin::catchItem() {
    const auto ball = _ball.lock();
    ball->obtainCoin();
}

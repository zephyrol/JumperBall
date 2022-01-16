/*
 * File: Coin.cpp
 * Author: Morgenthaler S
 *
 * Created on 27 mars 2020, 20:49
 */

#include "Coin.h"

Coin::Coin(const JBTypes::vec3ui& position, const JBTypes::Dir& direction, const Ball_sptr &ball):
    Bonus(position, direction, 100, ball) {
}

vecCstShape_sptr Coin::getShapes() const {

    const auto coinShape = std::make_shared<const Shape>(
        Shape::Aspect::Cylinder,
        JBTypes::Color::Orange,
        std::initializer_list<Transformation>({
            Transformation(Transformation::Type::Translation, { 0.f, -0.5f, 0.f}),
            Transformation(Transformation::Type::Scale, { 0.3f, 0.05f, 0.3f }),
            Transformation(Transformation::Type::Rotation, { static_cast<float>(M_PI / 2), 0.f, 0.f })
        })
    );

    return { coinShape };
}

void Coin::catchItem() {
    const auto ball = _ball.lock();
    ball->obtainCoin();
}

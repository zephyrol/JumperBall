/*
 * File: Coin.cpp
 * Author: Morgenthaler S
 *
 * Created on 27 mars 2020, 20:49
 */

#include "Coin.h"

Coin::Coin(const JBTypes::vec3ui& position, const JBTypes::Dir& direction):
    Bonus(position, direction, 100) {
}

vecCstShape_sptr Coin::getShapes() const {
    const JBTypes::vec3f localRotation { static_cast<float>(M_PI / 2), 0.f, 0.f };
    const JBTypes::vec3f localTranslation { 0.f, 0.f, -0.025f };

    const auto coinShape = std::make_shared<const Shape>(
        Shape::Aspect::Cylinder,
        JBTypes::Color::Orange,
        direction(),
        localRotation,
        JBTypesMethods::add(get3DPosition(), localTranslation)
    );

    return { coinShape };
}

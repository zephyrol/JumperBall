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
    return { std::make_shared<const Shape>(
        Shape::Aspect::Cylinder,
        JBTypes::Color::Orange,
        direction(),
        JBTypesMethods::add(get3DPosition(),
                            std::initializer_list <float>({ 0.f, 0.f, -0.025f}),
                            std::initializer_list <float>({ { 0.3f, 0.05f, 0.3f})
    )};
}


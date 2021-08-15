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

std::vector<Shape> Coin::getShapes() const {
    return { Shape(
        Shape::Aspect::Cylinder,
        JBTypes::Color::Orange,
        direction(),
        get3DPosition(),
        { 0.3f, 0.05f, -0.025f})};
}

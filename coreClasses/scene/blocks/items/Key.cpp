/*
 * File: Key.cpp
 * Author: Morgenthaler S
 *
 * Created on 27 mars 2020, 20:45
 */

#include "Key.h"

Key::Key(const JBTypes::vec3ui& position, const JBTypes::Dir& direction):
    Item(position, direction) {
}

std::vector<Shape> Key::getShapes() const {
    std::cout << "get shapes" << std::endl;
    return {
        Shape(
            Shape::Aspect::Sphere,
            JBTypes::Color::Yellow,
            direction(),
            { 0.f, 0.175f, 0.f},
            { 0.09f, 0.075f, 0.050f}
        ),
        Shape(
            Shape::Aspect::Cube,
            JBTypes::Color::Yellow,
            direction(),
            { -0.025f, -0.175f, -0.025f },
            { 0.09f, 0.075f, 0.050f }
        ),
        Shape(
            Shape::Aspect::Cube,
            JBTypes::Color::Yellow,
            direction(),
            { -0.025f, -0.075f, -0.025f },
            { 0.1f, 0.05f, 0.05f }
        ),
        Shape(
            Shape::Aspect::Cube,
            JBTypes::Color::Yellow,
            direction(),
            { -0.025f, -0.175f, -0.025f },
            { 0.1f, 0.05f, 0.05f }
        )
    };
}

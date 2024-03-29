/*
 * File: Key.cpp
 * Author: Morgenthaler S
 *
 * Created on 27 mars 2020, 20:45
 */

#include "Key.h"
#include <initializer_list>

Key::Key(const JBTypes::vec3ui &position, const JBTypes::Dir &direction, const Ball_sptr &ball) :
    InteractiveItem(position, direction, ball) {
}

vecCstShape_sptr Key::getShapes() const {

    const auto rotation = Transformation(
        Transformation::Type::Rotation,
        JBTypesMethods::rotationVectorUpToDir(direction())
    );

    const auto sphereHead = std::make_shared<const Shape>(
        Shape::Aspect::Sphere,
        JBTypes::Color::Yellow,
        std::initializer_list<Transformation>(
            {
                Transformation(Transformation::Type::Scale,
                               {0.09f, 0.075f, 0.050f}),
                Transformation(Transformation::Type::Translation,
                               {0.f, 0.175f, 0.f}),
                               rotation
            }
        )
    );

    const auto cubeBody1 = std::make_shared<const Shape>(
        Shape::Aspect::Cube,
        JBTypes::Color::Yellow,
        std::initializer_list<Transformation>(
            {
                Transformation(Transformation::Type::Scale,
                               {0.05f, 0.3f, 0.050f}),
                Transformation(Transformation::Type::Translation,
                               {0.f, -0.025f, 0.f}),
                rotation
            }
        )
    );

    const auto cubeBody2 = std::make_shared<const Shape>(
        Shape::Aspect::Cube,
        JBTypes::Color::Yellow,
        std::initializer_list<Transformation>(
            {
                Transformation(Transformation::Type::Scale,
                               {0.1f, 0.05f, 0.05f}),
                Transformation(Transformation::Type::Translation,
                               {0.025f, -0.05f, 0.f}),
                rotation
            }
        )
    );

    const auto cubeBody3 = std::make_shared<const Shape>(
        Shape::Aspect::Cube,
        JBTypes::Color::Yellow,
        std::initializer_list<Transformation>(
            {
                Transformation(Transformation::Type::Scale,
                               {0.1f, 0.05f, 0.05f}),
                Transformation(Transformation::Type::Translation,
                               {0.025f, -0.15f, 0.f}),
                rotation
            }
        )
    );

    return {sphereHead, cubeBody1, cubeBody2, cubeBody3};
}

void Key::catchItem() {
    const auto ball = _ball.lock();
    ball->obtainKey();
}

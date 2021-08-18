/*
 * File: Key.cpp
 * Author: Morgenthaler S
 *
 * Created on 27 mars 2020, 20:45
 */

#include "Key.h"
#include <initializer_list>

Key::Key(const JBTypes::vec3ui& position, const JBTypes::Dir& direction):
    Item(position, direction) {
}

vecCstShape_sptr Key::getShapes() const {
    std::cout << "get shapes" << std::endl;

    const JBTypes::vec3f positionSphere {0.f, 0.175f, 0.f};
    const JBTypes::vec3f scaleSphere {0.09f, 0.075f, 0.050f};
    const auto sphereHead = std::make_shared<const Shape>(
        Shape::Aspect::Sphere,
        JBTypes::Color::Yellow,
        direction(),
        positionSphere,
        scaleSphere
    );

    const JBTypes::vec3f positionCube1 { -0.025f, -0.175f, -0.025f };
    const JBTypes::vec3f scaleCube1 { 0.05f, 0.3f, 0.050f };
    const auto cubeBody1 = std::make_shared<const Shape>(
        Shape::Aspect::Cube,
        JBTypes::Color::Yellow,
        direction(),
        positionCube1,
        scaleCube1
    );

    const JBTypes::vec3f positionCube2 {-0.025f, -0.075f, -0.025f};
    const JBTypes::vec3f scaleCube2 {0.1f, 0.05f, 0.05f};
    const auto cubeBody2 = std::make_shared<const Shape>(
        Shape::Aspect::Cube,
        JBTypes::Color::Yellow,
        direction(),
        positionCube2,
        scaleCube2
    );

    const JBTypes::vec3f positionCube3 {-0.025f, -0.175f, -0.025f};
    const JBTypes::vec3f scaleCube3 {0.1f, 0.05f, 0.05f};
    const auto cubeBody3 = std::make_shared<const Shape>(
        Shape::Aspect::Cube,
        JBTypes::Color::Yellow,
        direction(),
        positionCube3,
        scaleCube3
    );

    return { sphereHead, cubeBody1, cubeBody2, cubeBody3 };
}

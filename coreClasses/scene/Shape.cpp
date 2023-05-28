//
// Created by S.Morgenthaler on 13/08/2021.
//

#include "Shape.h"

#include <utility>

Shape::Shape(const Shape::Aspect &aspect, const JBTypes::Color &color,
             std::vector<Transformation> &&transformations) :
    _aspect(aspect),
    _color(color),
    _transformations(std::move(transformations)) {

}

const Shape::Aspect &Shape::aspect() const {
    return _aspect;
}

const JBTypes::Color &Shape::color() const {
    return _color;
}

const std::vector<Transformation> &Shape::transformations() const {
    return _transformations;
}

Transformation Shape::getVerticalCylinderRotation(const JBTypes::Dir &direction) {
    const auto rotationVector = [](const JBTypes::Dir &direction) -> JBTypes::vec3f {

        constexpr auto fPI = static_cast <float>(M_PI);
        constexpr auto fPI2 = static_cast <float>(M_PI_2);

        switch (direction) {
            case JBTypes::Dir::North:
                return {0.f, 0.f, 0.f};
            case JBTypes::Dir::South:
                return {fPI, 0.f, 0.f};
            case JBTypes::Dir::East:
                return {fPI2, fPI2, 0.f};
            case JBTypes::Dir::West:
                return {-fPI2, -fPI2, 0.f};
            case JBTypes::Dir::Up:
                return {fPI2, 0.f, 0.f};
            case JBTypes::Dir::Down:
                return {-fPI2, 0.f, 0.f};
            default:
                return {0.0f, 0.0f, 0.0f};
        }
    }(direction);
    return Transformation(Transformation::Type::Rotation, rotationVector);
}


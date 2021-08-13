//
// Created by seb on 13/08/2021.
//

#include "Shape.h"

Shape::Shape(
    const Shape::Aspect &aspect,
    const JBTypes::Color& color,
    const JBTypes::Dir &dir,
    const JBTypes::vec3f &position,
    const JBTypes::vec3f &scale
) : _aspect(aspect),
    _color(color),
    _dir(dir),
    _position(position),
    _scale(scale) {
}

const Shape::Aspect& Shape::aspect() const {
    return _aspect;
}

const JBTypes::Dir &Shape::dir() const {
    return _dir;
}

const JBTypes::vec3f &Shape::position() const {
    return _position;
}

const JBTypes::vec3f &Shape::scale() const {
    return _scale;
}

const JBTypes::Color &Shape::color() const {
    return _color;
}


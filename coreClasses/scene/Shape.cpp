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
    _dir(new JBTypes::Dir(dir)),
    _rotation(nullptr),
    _position(position),
    _scale(scale) {
}



Shape::Shape(
    const Shape::Aspect &aspect,
    const JBTypes::Color &color,
    const JBTypes::vec3f &rotation,
    const JBTypes::vec3f &position,
    const JBTypes::vec3f &scale
) : _aspect(aspect),
    _color(color),
    _dir(nullptr),
    _rotation(new JBTypes::vec3f(rotation)),
    _position(position),
    _scale(scale)
{

}

Shape::Shape(
    const Shape::Aspect &aspect,
    const JBTypes::Color &color,
    const JBTypes::Dir &dir,
    const JBTypes::vec3f &rotation,
    const JBTypes::vec3f &position,
    const JBTypes::vec3f &scale
):  _aspect(aspect),
    _color(color),
    _dir(new JBTypes::Dir(dir)),
    _rotation(new JBTypes::vec3f(rotation)),
    _position(position),
    _scale(scale)
{
}


const Shape::Aspect& Shape::aspect() const {
    return _aspect;
}

const std::unique_ptr<const JBTypes::Dir>& Shape::dir() const {
    return _dir;
}

const std::unique_ptr<const JBTypes::vec3f>& Shape::rotation() const {
    return _rotation;
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

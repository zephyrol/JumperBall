/*
 * File:   Geometry.cpp
 * Author: Morgenthaler S
 *
 * Created on April 27, 2021, 8:20 PM
 */

#include "Geometry.h"

#include <utility>

Geometry::Geometry(
    const Shape &shape,
    ShapeTranslation &&translation,
    ShapeRotation &&rotation,
    ShapeScale &&scale,
    CustomUvs&& customUvs
) :
    _shape(shape),
    _translation(translation),
    _rotation(rotation),
    _scale(scale),
    _customUvs(std::move(customUvs)) {
}

const Geometry::ShapeTranslation &Geometry::getTranslation() const {
    return _translation;
}

const Geometry::ShapeRotation &Geometry::getRotation() const {
    return _rotation;
}

const Geometry::ShapeScale &Geometry::getScale() const {
    return _scale;
}

const Geometry::Shape &Geometry::getShape() const {
    return _shape;
}

const Geometry::CustomUvs &Geometry::getCustomUvs() const {
    return _customUvs;
}

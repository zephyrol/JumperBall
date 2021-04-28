/*
 * File:   Geometry.cpp
 * Author: Morgenthaler S
 *
 * Created on April 27, 2021, 8:20 PM
 */

#include "Geometry.h"

Geometry::Geometry(
    const Shape& shape,
    const ShapeTranslation& translation,
    const ShapeRotation& rotation,
    const ShapeScale& scale):
    _shape(shape),
    _translation(translation),
    _rotation(rotation),
    _scale(scale) {

}

const Geometry::ShapeTranslation& Geometry::getTranslation() const {
    return _translation;
}

const Geometry::ShapeRotation& Geometry::getRotation() const {
    return _rotation;
}

const Geometry::ShapeScale& Geometry::getScale() const {
    return _scale;
}

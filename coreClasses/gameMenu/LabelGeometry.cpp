/*
 * File:   LabelGeometry.cpp
 * Author: Morgenthaler S
 *
 * Created on April 27, 2021, 8:20 PM
 */

#include "LabelGeometry.h"

#include <utility>

LabelGeometry::LabelGeometry(
    const Shape &shape,
    ShapeTranslation &&translation,
    ShapeRotation &&rotation,
    ShapeScale &&scale,
    const JBTypes::Color& color,
    CustomUvs&& customUvs
) :
    _shape(shape),
    _color(color),
    _translation(translation),
    _rotation(rotation),
    _scale(scale),
    _customUvs(std::move(customUvs)) {
}

const LabelGeometry::ShapeTranslation &LabelGeometry::getTranslation() const {
    return _translation;
}

const LabelGeometry::ShapeRotation &LabelGeometry::getRotation() const {
    return _rotation;
}

const LabelGeometry::ShapeScale &LabelGeometry::getScale() const {
    return _scale;
}

const LabelGeometry::Shape &LabelGeometry::getShape() const {
    return _shape;
}

const LabelGeometry::CustomUvs &LabelGeometry::getCustomUvs() const {
    return _customUvs;
}

const JBTypes::Color &LabelGeometry::getColor() const {
    return _color;
}

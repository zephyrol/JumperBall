/*
 * File:   LabelGeometry.h
 * Author: Morgenthaler S
 *
 * Created on April 27, 2021, 8:20 PM
 */

#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "system/Types.h"

class LabelGeometry;

using vecLabelGeometry = std::vector<LabelGeometry>;

class LabelGeometry {
public:
    enum class Shape { Triangle, Quad };
    using ShapeTranslation = std::array<float, 3>;
    using ShapeRotation = std::array<float, 3>;
    using ShapeScale = std::array<float, 3>;
    using CustomUvs = std::vector<std::array<float, 2> >;

    LabelGeometry(
        const Shape &shape,
        ShapeTranslation &&translation,
        ShapeRotation &&rotation,
        ShapeScale &&scale,
        const JBTypes::Color& color,
        CustomUvs&& customUvs = {}
    );

    const Shape &getShape() const;

    const ShapeTranslation &getTranslation() const;

    const ShapeRotation &getRotation() const;

    const ShapeScale &getScale() const;

    const CustomUvs &getCustomUvs() const;

    const JBTypes::Color &getColor() const;


private:
    const Shape _shape;
    const JBTypes::Color _color;
    const ShapeTranslation _translation;
    const ShapeRotation _rotation;
    const ShapeScale _scale;
    const CustomUvs _customUvs;
};

#endif /* GEOMETRY_H */

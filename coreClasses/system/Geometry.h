/*
 * File:   Geometry.h
 * Author: Morgenthaler S
 *
 * Created on April 27, 2021, 8:20 PM
 */

#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "Types.h"

class Geometry;

using vecGeometry = std::vector<Geometry>;


class Geometry {
public:
    enum class Shape {
        Triangle, Quad, Cube, Sphere, Cylinder, Pyramid, Socle
    };
    using ShapeTranslation = std::array<float, 3>;
    using ShapeRotation = std::array<float, 3>;
    using ShapeScale = std::array<float, 3>;
    using CustomUvs = std::vector<std::array<float, 2> >;

    Geometry(
        const Shape &shape,
        const ShapeTranslation &translation,
        const ShapeRotation &rotation,
        const ShapeScale &scale,
        const CustomUvs &customUvs = {}
    );

    const Shape &getShape() const;

    const ShapeTranslation &getTranslation() const;

    const ShapeRotation &getRotation() const;

    const ShapeScale &getScale() const;

    const CustomUvs &getCustomUvs() const;


private:
    const Shape _shape;
    const ShapeTranslation _translation;
    const ShapeRotation _rotation;
    const ShapeScale _scale;
    const CustomUvs _customUvs;
};

#endif /* GEOMETRY_H */

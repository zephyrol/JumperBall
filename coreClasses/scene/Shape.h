//
// Created by seb on 13/08/2021.
//

#ifndef JUMPERBALLAPPLICATION_SHAPE_H
#define JUMPERBALLAPPLICATION_SHAPE_H


#include <system/Types.h>

// TODO: merge it with Geometry class
class Shape {
public:

    enum class Aspect { Cube, Cylinder, Sphere, Pyramid };

    explicit Shape(
        const Aspect& aspect,
        const JBTypes::Color& color,
        const JBTypes::Dir& dir,
        const JBTypes::vec3f& position,
        const JBTypes::vec3f& scale
    );

    const Aspect& aspect() const;
    const JBTypes::Color& color() const;
    const JBTypes::Dir& dir() const;
    const JBTypes::vec3f& position() const;
    const JBTypes::vec3f& scale() const;

private:
    const Aspect _aspect;
    const JBTypes::Color _color;
    const JBTypes::Dir _dir;
    const JBTypes::vec3f _position;
    const JBTypes::vec3f _scale;
};

#endif //JUMPERBALLAPPLICATION_SHAPE_H

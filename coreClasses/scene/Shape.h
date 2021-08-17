//
// Created by seb on 13/08/2021.
//

#ifndef JUMPERBALLAPPLICATION_SHAPE_H
#define JUMPERBALLAPPLICATION_SHAPE_H


#include <system/Types.h>

// TODO: merge it with Geometry class

class Shape;
using Shape_sptr = std::shared_ptr <Shape>;
using CstShape_sptr = std::shared_ptr <const Shape>;
using vecCstShape_sptr = std::vector <CstShape_sptr>;
using vecShape_sptr = std::vector <Shape_sptr>;

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

    explicit Shape(
        const Aspect& aspect,
        const JBTypes::Color& color,
        const JBTypes::vec3f& rotation,
        const JBTypes::vec3f& position,
        const JBTypes::vec3f& scale
    );

    explicit Shape(
        const Aspect& aspect,
        const JBTypes::Color& color,
        const JBTypes::Dir& dir,
        const JBTypes::vec3f& rotation,
        const JBTypes::vec3f& position,
        const JBTypes::vec3f& scale
    );

    const Aspect& aspect() const;
    const JBTypes::Color& color() const;
    const std::unique_ptr<const JBTypes::Dir>& dir() const;
    const std::unique_ptr<const JBTypes::vec3f>& rotation() const;
    const JBTypes::vec3f& position() const;
    const JBTypes::vec3f& scale() const;

private:
    const Aspect _aspect;
    const JBTypes::Color _color;
    const std::unique_ptr<const JBTypes::Dir> _dir;
    const std::unique_ptr<const JBTypes::vec3f> _rotation;
    const JBTypes::vec3f _position;
    const JBTypes::vec3f _scale;
};

#endif //JUMPERBALLAPPLICATION_SHAPE_H

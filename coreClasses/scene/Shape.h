//
// Created by seb on 13/08/2021.
//

#ifndef JUMPERBALLAPPLICATION_SHAPE_H
#define JUMPERBALLAPPLICATION_SHAPE_H


#include <system/Types.h>
#include <scene/blocks/Transformation.h>
#include <list>

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
        std::list<Transformation>&& transformations
    );

    const Aspect& aspect() const;
    const JBTypes::Color& color() const;
    const std::list<Transformation>& transformations() const;

private:
    const Aspect _aspect;
    const JBTypes::Color _color;
    const std::list<Transformation> _transformations;
};

#endif //JUMPERBALLAPPLICATION_SHAPE_H

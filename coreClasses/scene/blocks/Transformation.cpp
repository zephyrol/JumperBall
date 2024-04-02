//
// Created by S.Morgenthaler on 21/08/2021.
//

#include "Transformation.h"

Transformation::Transformation(const Transformation::Type &type, const glm::vec3 &value):
_type(type),
_value(value)
{
}

const Transformation::Type &Transformation::type() const {
    return _type;
}

const glm::vec3& Transformation::value() const {
    return _value;
}

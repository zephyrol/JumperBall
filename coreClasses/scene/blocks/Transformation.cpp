//
// Created by seb on 21/08/2021.
//

#include "Transformation.h"

Transformation::Transformation(const Transformation::Type &type, const JBTypes::vec3f &value):
_type(type),
_value(value)
{
}

const Transformation::Type &Transformation::type() const {
    return _type;
}

const JBTypes::vec3f& Transformation::value() const {
    return _value;
}

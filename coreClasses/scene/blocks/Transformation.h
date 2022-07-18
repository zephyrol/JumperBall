//
// Created by seb on 21/08/2021.
//

#ifndef JUMPERBALLAPPLICATION_TRANSFORMATION_H
#define JUMPERBALLAPPLICATION_TRANSFORMATION_H


#include <system/Types.h>

class Transformation {

public :
    enum class Type {
        Translation, Rotation, Scale
    };

    explicit Transformation(const Type &type, const JBTypes::vec3f &value);

    const Type &type() const;

    const JBTypes::vec3f &value() const;

private:
    const Type _type;
    const JBTypes::vec3f _value;
};


#endif //JUMPERBALLAPPLICATION_TRANSFORMATION_H

//
// Created by S.Morgenthaler on 21/08/2021.
//

#ifndef JUMPERBALLAPPLICATION_TRANSFORMATION_H
#define JUMPERBALLAPPLICATION_TRANSFORMATION_H


#include <system/Types.h>

class Transformation {

public :
    enum class Type {
        Translation, Rotation, Scale
    };

    explicit Transformation(const Type &type, const glm::vec3 &value);

    const Type &type() const;

    const glm::vec3 &value() const;

private:
    const Type _type;
    const glm::vec3 _value;
};


#endif //JUMPERBALLAPPLICATION_TRANSFORMATION_H

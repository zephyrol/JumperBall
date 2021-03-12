/*
 * File: Object.cpp
 * Author: Morgenthaler S
 *
 * Created on 27 mars 2020, 19:40
 */

#include "Object.h"
#include <math.h>

Object::Object(
    const JBTypes::vec3ui& position,
    const JBTypes::Dir& direction,
    const CategoryOfObjects& category
    ):
    _position(position),
    _direction(direction),
    _gotten(false),
    _creationTime(JBTypesMethods::getTimePointMSNow()),
    _obtainingTime(),
    _category(category) {
}

const Object::CategoryOfObjects& Object::getCategory() const {
    return _category;
}

const JBTypes::vec3ui& Object::position() const {
    return _position;
}

const JBTypes::Dir& Object::direction() const {
    return _direction;
}

bool Object::isGotten() const {
    return _gotten;
}

float Object::getTimeSinceObtaining() const {
    return _gotten
           ? JBTypesMethods::getTimeSecondsSinceTimePoint(_obtainingTime)
           : 0;
}

float Object::getTimeSinceCreation() const {
    return JBTypesMethods::getTimeSecondsSinceTimePoint(_creationTime);
}

void Object::catchingTest (const JBTypes::vec3f& objectPosition,
                           const JBTypes::vec3f& entityPosition,
                           float radiusEntity) {
    const float distance =
        JBTypesMethods::distance(objectPosition, entityPosition);
    if (distance < radiusEntity + radiusBoundingSphere) {
        _obtainingTime = JBTypesMethods::getTimePointMSNow();
        _gotten = true;
    }

}

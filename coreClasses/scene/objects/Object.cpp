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
    _timeOfCreation(JBTypesMethods::getTimePointMSNow()),
    _timeOfObtaining(),
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

const JBTypes::timePointMs& Object::timeOfObtaining() const {
    return _timeOfObtaining;
}

const JBTypes::timePointMs& Object::timeOfCreation() const {
    return _timeOfCreation;
}

void Object::catchingTest (const JBTypes::vec3f& objectPosition,
                           const JBTypes::vec3f& entityPosition,
                           float radiusEntity) {
    const float distance =
        JBTypesMethods::distance(objectPosition, entityPosition);
    if (distance < radiusEntity + radiusBoundingSphere) {
        _timeOfObtaining = JBTypesMethods::getTimePointMSNow();
        _gotten = true;
    }

}

/*
 * File: ObjectState.cpp
 * Author: Morgenthaler S
 *
 * Created on 19 septembre 2020, 07h50
 */
#include "ObjectState.h"

ObjectState::ObjectState(const Object& object):
    _object(object),
    _isGotten(object.isGotten()),
    _timeOfObtaining(object.timeOfObtaining()) {
}

void ObjectState::update() {
    _isGotten = _object.isGotten();
    _timeOfObtaining = _object.timeOfObtaining();
}

bool ObjectState::isGotten() const {
    return _isGotten;
}

const JBTypes::timePointMs& ObjectState::timeOfObtaining() const {
    return _timeOfObtaining;
}

std::vector <float> ObjectState::getStaticFloatValues() const {
    return { static_cast <float>(_object.direction()) };
}

std::vector <JBTypes::vec3f> ObjectState::getStaticVec3fValues() const {
    constexpr float offset = 0.5;
    const auto uintPosition = _object.position();
    const JBTypes::vec3f position = {
        static_cast <float>(uintPosition.at(0)) + offset,
        static_cast <float>(uintPosition.at(1)) + offset,
        static_cast <float>(uintPosition.at(2)) + offset
    };
    return { position };
}

std::map <std::string, float> ObjectState::getDynamicFloats() const {
    return {
        { "creationTime", JBTypesMethods::getTimeSecondsSinceTimePoint(_object.timeOfCreation()) },
        { "obtainingTime", JBTypesMethods::getTimeSecondsSinceTimePoint(_timeOfObtaining) }
    };
}

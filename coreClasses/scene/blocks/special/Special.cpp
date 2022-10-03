/*
 * File: Special.cpp
 * Author: Morgenthaler S
 *
 * Created on 7 novembre 2020, 10:42
 */
#include "Special.h"

Special::Special(
    const JBTypes::Color& color,
    const JBTypes::Dir& dir,
    const JBTypes::vec3ui& position,
    bool isActivated
):  
    _creationTime(JBTypesMethods::getTimePointMSNow()),
    _direction(dir),
    _color(color),
    _colorAttributeName("is" + JBTypesMethods::colorToString(_color) + "Activated"),
    _position(position),
    _isActivated(isActivated) {
}

const JBTypes::Color& Special::getColor() const {
    return _color;
}

float Special::getTimeSinceCreation() const {
    return JBTypesMethods::getTimeSecondsSinceTimePoint(_creationTime);
}

const JBTypes::Dir& Special::direction() const {
    return _direction;
}

const JBTypes::vec3ui& Special::position() const {
    return _position;
}

bool Special::isActivated() const {
    return _isActivated;
}

bool Special::isAnimated() const {
    return false;
}

void Special::switchOnOff() {
    _isActivated = !_isActivated;
}

Displayable::GlobalState Special::getGlobalState() const {
    return Displayable::GlobalState::United;
}

Displayable::StaticValues <float> Special::getStaticFloatValues() const {
    return { static_cast<float>(_direction), static_cast<float>(_color), static_cast<float>(isAnimated()) };
}

Displayable::StaticValues <JBTypes::vec3f> Special::getStaticVec3fValues() const {
    const JBTypes::vec3f position = {
        static_cast <float>(_position.at(0)),
        static_cast <float>(_position.at(1)),
        static_cast <float>(_position.at(2))
    };
    return { position };
}

Displayable::DynamicValues <float> Special::getDynamicFloats() const {
    return {
        { "creationTime", getTimeSinceCreation()},
        { _colorAttributeName, _isActivated }
    };
}

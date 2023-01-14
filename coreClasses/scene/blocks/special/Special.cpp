/*
 * File: Special.cpp
 * Author: Morgenthaler S
 *
 * Created on 7 novembre 2020, 10:42
 */
#include "Special.h"

Special::Special(
    CstChronometer_sptr chronometer,
    const JBTypes::Color &color,
    const JBTypes::Dir &dir,
    const JBTypes::vec3ui &position,
    bool isActivated
) :
    _chronometer(std::move(chronometer)),
    _direction(dir),
    _color(color),
    _colorAttributeName("is" + JBTypesMethods::colorToString(_color) + "Activated"),
    _position(position),
    _isActivated(isActivated) {
}

const JBTypes::Color &Special::getColor() const {
    return _color;
}

const JBTypes::Dir &Special::direction() const {
    return _direction;
}

const JBTypes::vec3ui &Special::position() const {
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

std::vector<unsigned char> Special::getStaticUnsignedByteValues() const {
    return {
        static_cast<unsigned char>(_direction),
        static_cast<unsigned char>(_color),
        static_cast<unsigned char>(isAnimated())
    };
}

Displayable::StaticValues<JBTypes::vec3f> Special::getStaticVec3fValues() const {
    const JBTypes::vec3f position = {
        static_cast <float>(_position.at(0)),
        static_cast <float>(_position.at(1)),
        static_cast <float>(_position.at(2))
    };
    return {position};
}

Displayable::DynamicNames Special::getDynamicFloatNames() const {
    return { "creationTime", _colorAttributeName };
}

Displayable::DynamicValues<float> Special::getDynamicFloatValues() const {
    return {
        _chronometer->getTime(),
        static_cast<float>(_isActivated) // TODO Use dynamic int ?
    };
}

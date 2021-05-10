/*
 * File: SpecialState.cpp
 * Author: Morgenthaler S
 *
 * Created on 14 novembre 2020, 18h09
 */
#include "SpecialState.h"

SpecialState::SpecialState(const Special& special):
    _special(special),
    _position(special.position()),
    _direction(static_cast <float>(special.direction())),
    _color(static_cast <float>(special.getColor())),
    _colorAttributeName("is" + JBTypesMethods::colorToString(special.getColor()) + "Activated"),
    _isAnimated(static_cast <float>(special.isAnimated())),
    _timeSinceCreation(special.getTimeSinceCreation()),
    _isActivated(special.isActivated()) {
}

State::GlobalState SpecialState::update() {
    _timeSinceCreation = _special.getTimeSinceCreation();
    _isActivated = _special.isActivated();
    return GlobalState::United;
}

State::StaticValues <float> SpecialState::getStaticFloatValues() const {
    return { _direction, _color, _isAnimated };
}

State::StaticValues <JBTypes::vec3f> SpecialState::getStaticVec3fValues() const {
    constexpr float offset = 0.5;
    const JBTypes::vec3f position = {
        static_cast <float>(_position.at(0)) + offset,
        static_cast <float>(_position.at(1)) + offset,
        static_cast <float>(_position.at(2)) + offset
    };
    return { position };
}

State::DynamicValues <float> SpecialState::getDynamicFloats() const {
    return {
        { "creationTime", _timeSinceCreation },
        { _colorAttributeName, _isActivated }
    };
}

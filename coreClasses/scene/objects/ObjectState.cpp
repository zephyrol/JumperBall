/*
 * File: ObjectState.cpp
 * Author: Morgenthaler S
 *
 * Created on 19 septembre 2020, 07h50
 */
#include "ObjectState.h"

ObjectState::ObjectState(const Object& object):
    _object(object),
    _position(object.position()),
    _direction(static_cast <float>(object.direction())),
    _timeSinceCreation(object.getTimeSinceCreation()),
    _timeSinceObtaining(object.getTimeSinceObtaining()) {
}

State::GlobalState ObjectState::update() {
    _timeSinceCreation = _object.getTimeSinceCreation();
    _timeSinceObtaining = _object.getTimeSinceObtaining();

    if (_object.isGotten()) {
        constexpr float thresholdThirdStep = 1.5;
        constexpr float durationThirdStep = 0.2;
        return _timeSinceObtaining < thresholdThirdStep + durationThirdStep
               ? State::GlobalState::Separate
               : State::GlobalState::Dead;
    }
    return State::GlobalState::United;
}

State::StaticValues <float> ObjectState::getStaticFloatValues() const {
    return { _direction };
}

State::StaticValues <JBTypes::vec3f> ObjectState::getStaticVec3fValues() const {
    constexpr float offset = 0.5;
    const JBTypes::vec3f position = {
        static_cast <float>(_position.at(0)) + offset,
        static_cast <float>(_position.at(1)) + offset,
        static_cast <float>(_position.at(2)) + offset
    };
    return { position };
}

State::DynamicValues <float> ObjectState::getDynamicFloats() const {
    return {
        { "creationTime", _timeSinceCreation },
        { "obtainingTime", _timeSinceObtaining }
    };
}

/*
 * File: StarState.cpp
 * Author: Morgenthaler S
 *
 * Created on 19 septembre 2020, 07h50
 */
#include "StarState.h"

StarState::StarState(const Star& star):
    State(
{
    { envSizeName, star.envSize() }
},
{
    { lightDirectionName, star.lightDirection() },
    { positionName, star.position() },
    { rotationCenterName, star.rotationCenter() },
}),
    _star(star),
    _rotation(star.getRotation()) {
}

State::GlobalState StarState::update() {
    _rotation = _star.getRotation();
    setExposableVec3f(lightDirectionName, _star.lightDirection());
    setExposableVec3f(positionName, _star.position());
    return State::GlobalState::United;
}

StarState::DynamicValues <float> StarState::getDynamicFloats() const {
    return {
        { "starDistance", _star.distance() },
        { "radius", _star.radius() },
        { "radiusInside", _star.radiusInside() },
        { "radiusOutside", _star.radiusOutside() },
    };
}

StarState::DynamicValues <JBTypes::vec3f> StarState::getDynamicVec3fs() const {
    return {
        { "initialDirection", _star.initialDirection() },
        { "rotationCenter", _star.rotationCenter() }
    };
}

StarState::DynamicValues <JBTypes::Quaternion> StarState::getDynamicQuaternions() const {
    return {
        { "rotationQuaternion", _rotation }
    };
}

const std::string StarState::lightDirectionName = "lightDirection";
const std::string StarState::positionName = "position";
const std::string StarState::rotationCenterName = "rotationCenter";
const std::string StarState::envSizeName = "envSize";

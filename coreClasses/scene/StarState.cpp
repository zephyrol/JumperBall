/*
 * File: StarState.cpp
 * Author: Morgenthaler S
 *
 * Created on 19 septembre 2020, 07h50
 */
#include "StarState.h"

StarState::StarState(const Star& star):
    _star(star),
    _rotation(star.getRotation()),
    _position(star.position()),
    _lightDirection(star.lightDirection()) {
}

ObjectState::GlobalState StarState::update() {
    _rotation = _star.getRotation();
    _position = _star.position();
    _lightDirection = _star.lightDirection();
    return ObjectState::GlobalState::United;
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

float StarState::getEnvSize() const {
    return _star.envSize();
}

const JBTypes::vec3f& StarState::getRotationCenter() const {
    return _star.rotationCenter();
}

const JBTypes::vec3f& StarState::getPosition() const {
    return _position;
}

const JBTypes::vec3f& StarState::getLightDirection() const {
    return _lightDirection;
}

const std::string StarState::lightDirectionName = "lightDirection";

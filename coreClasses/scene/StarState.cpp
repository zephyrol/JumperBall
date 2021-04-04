/*
 * File: StarState.cpp
 * Author: Morgenthaler S
 *
 * Created on 19 septembre 2020, 07h50
 */
#include "StarState.h"

StarState::StarState(const Star& star):
    _star(star),
    _lightDirection(star.lightDirection()),
    _position(star.position()) {
}

State::GlobalState StarState::update() {
    _rotation = _star.getRotation();
    _lightDirection = _star.lightDirection();
    _position = _star.position();
    return State::GlobalState::United;
}

const JBTypes::vec3f& StarState::lightDirection() const {
    return _lightDirection;
}

const JBTypes::vec3f& StarState::position() const {
    return _position;
}

std::map <std::string, float> StarState::getDynamicFloats() const {
    return {
        { "starDistance", _star.distance() },
        { "radius", _star.radius() },
        { "radiusInside", _star.radiusInside() },
        { "radiusOutside", _star.radiusOutside() },
    };
}

std::map <std::string, JBTypes::vec3f> StarState::getDynamicVec3fs() const {
    return {
        { "initialDirection", _star.initialDirection() },
        { "rotationCenter", _star.rotationCenter() }
    };
}

std::map <std::string, JBTypes::Quaternion> StarState::getDynamicQuaternions() const {
    return {
        { "rotationQuaternion", _rotation }
    };
}

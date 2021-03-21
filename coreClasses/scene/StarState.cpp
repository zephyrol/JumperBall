/*
 * File: StarState.cpp
 * Author: Morgenthaler S
 *
 * Created on 19 septembre 2020, 07h50
 */
#include "StarState.h"

StarState::StarState(const Star& star):
    _star(star),
    _timeSinceCreation(star.getTimeSinceCreation()),
    _lightDirection(star.lightDirection()) {
}

State::GlobalState StarState::update() {
    _lightDirection = _star.lightDirection();
    _timeSinceCreation = _star.getTimeSinceCreation();
    return State::GlobalState::United;
}

const JBTypes::vec3f& StarState::lightDirection() const {
    return _lightDirection;
}

std::map <std::string, float> StarState::getDynamicFloats() const {
    return {
        { "distance", _star.distance() },
        { "radius", _star.radius() },
        { "creationTime", _timeSinceCreation },
        { "radiusInside", _star.radiusInside() },
        { "radiusOutside", _star.radiusOutside() },
    };
}

std::map <std::string, JBTypes::vec3f> StarState::getDynamicVec3fs() const {
    return {
        { "rotationCenter", _star.rotationCenter() }
    };
}

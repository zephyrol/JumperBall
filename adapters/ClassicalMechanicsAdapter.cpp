/*
 * File: ClassicalMechanicsAdapter.cpp
 * Author: Morgenthaler S
 * Description: Implementation of ClassicalMechanicsAdapter
 */

#include "ClassicalMechanicsAdapter.h"

ClassicalMechanicsAdapter::ClassicalMechanicsAdapter(
    float ballRadius,
    float speedCoefficient
) : _mechanics(ballRadius, speedCoefficient) {
}

ClassicalMechanicsAdapter::ClassicalMechanicsAdapter(
    float ballRadius,
    float jumpDistance,
    float v0y,
    float speedCoefficient
) : _mechanics(ballRadius, jumpDistance, v0y, speedCoefficient) {
}

float ClassicalMechanicsAdapter::getJumpDistance() const {
    return _mechanics.getJumpDistance();
}

IPhysicsEngine::Vector2D ClassicalMechanicsAdapter::getVelocity(float t) const {
    const auto velocity = _mechanics.getVelocity(t);
    return {velocity.x, velocity.y};
}

IPhysicsEngine::Vector2D ClassicalMechanicsAdapter::getPosition(float t) const {
    const auto position = _mechanics.getPosition(t);
    return {position.x, position.y};
}

float ClassicalMechanicsAdapter::getTimeToGetDestination() const {
    return _mechanics.getTimeToGetDestination();
}

void ClassicalMechanicsAdapter::timesShock(const std::vector<float>& v) {
    _mechanics.timesShock(v);
}

void ClassicalMechanicsAdapter::addShockFromPosition(float pos) {
    _mechanics.addShockFromPosition(pos);
}


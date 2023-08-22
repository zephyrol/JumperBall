//
// Enemy.cpp
// JumperBallCore
//
// Created by S Morgenthaler on 08/10/2020.
//

#include "Enemy.h"

Enemy::Enemy(
    const JBTypes::vec3ui &initialPosition,
    const JBTypes::Dir &direction,
    float size,
    size_t length,
    bool isActivated
) :
    _direction(direction),
    _size(size),
    _length(length),
    _initialPosition(init3DPosition(initialPosition)),
    _position(_initialPosition),
    _isActivated(isActivated),
    _scale(isActivated ? scaleActivated : scaleDisable) {
}

JBTypes::Color Enemy::getColor() const {
    return JBTypes::Color::None;
}

float Enemy::size() const {
    return _size;
}

size_t Enemy::length() const {
    return _length;
}

void Enemy::switchOnOff() {
}

JBTypes::vec3f Enemy::init3DPosition(const JBTypes::vec3ui &initialPosition) const {

    const JBTypes::Dir &currentDir = _direction;
    const JBTypes::vec3f vecDir = JBTypesMethods::directionAsVector(currentDir);

    const JBTypes::vec3f posWorld = {
        static_cast <float>(initialPosition.at(0)),
        static_cast <float>(initialPosition.at(1)),
        static_cast <float>(initialPosition.at(2))
    };

    constexpr float offsetBlock = 0.5f;
    const float offsetToApply = offsetBlock + _size;
    return {
        posWorld.x + vecDir.x * offsetToApply,
        posWorld.y + vecDir.y * offsetToApply,
        posWorld.z + vecDir.z * offsetToApply
    };
}

const JBTypes::vec3f &Enemy::position() const {
    return _position;
}

const JBTypes::Dir &Enemy::direction() const {
    return _direction;
}

Displayable::DynamicNames Enemy::getDynamicVec3fNames() const {
    return {"translation", "scale"};
}

Displayable::DynamicValues<JBTypes::vec3f> Enemy::getDynamicVec3fValues() const {
    return { _position, _scale };
}

void Enemy::update() {
}

const JBTypes::vec3f Enemy::scaleActivated = {1.f, 1.f, 1.f};
const JBTypes::vec3f Enemy::scaleDisable = {0.f, 0.f, 0.f};

std::string Enemy::getDynamicGroupHash() const {
    return "enemy;" + std::to_string(_initialPosition.x) + "," + std::to_string(_initialPosition.y) + ","
           + std::to_string(_initialPosition.z) + "," + std::to_string(static_cast<int>(_direction));
}

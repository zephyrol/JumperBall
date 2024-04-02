//
// Enemy.cpp
// JumperBallCore
//
// Created by S Morgenthaler on 08/10/2020.
//

#include "Enemy.h"

Enemy::Enemy(
    const glm::u32vec3 &initialPosition,
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

glm::vec3 Enemy::init3DPosition(const glm::u32vec3 &initialPosition) const {

    const JBTypes::Dir &currentDir = _direction;
    const glm::vec3 vecDir = JBTypesMethods::directionAsVector(currentDir);

    const glm::vec3 posWorld = {
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

const glm::vec3 &Enemy::position() const {
    return _position;
}

const JBTypes::Dir &Enemy::direction() const {
    return _direction;
}

Displayable::DynamicValues<glm::vec3> Enemy::getDynamicVec3fValues() const {
    return { _position, _scale };
}

void Enemy::update() {
}

const glm::vec3 Enemy::scaleActivated = {1.f, 1.f, 1.f};
const glm::vec3 Enemy::scaleDisable = {0.f, 0.f, 0.f};

std::string Enemy::getDynamicGroupHash() const {
    return "enemy;" + std::to_string(_initialPosition.x) + "," + std::to_string(_initialPosition.y) + ","
           + std::to_string(_initialPosition.z) + "," + std::to_string(static_cast<int>(_direction));
}

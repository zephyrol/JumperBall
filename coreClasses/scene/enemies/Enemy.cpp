//
// Enemy.cpp
// JumperBallCore
//
// Created by S Morgenthaler on 08/10/2020.
//

#include "Enemy.h"

Enemy::Enemy(
    const JBTypes::vec3ui& initialPosition,
    const JBTypes::Dir& direction,
    float size,
    size_t length,
    const std::array <float, 9>& transform
):
    _creationTime(JBTypesMethods::getTimePointMSNow()),
    _direction(direction),
    _size(size),
    _length(length),
    _intersectionTime(),
    _hasHit(false),
    _initialPosition(init3DPosition(initialPosition)),
    _position(_initialPosition),
    _transform(transform) {
}

JBTypes::Color Enemy::getColor() const {
    return JBTypes::Color::None;
}

const JBTypes::timePointMs& Enemy::creationTime() const {
    return _creationTime;
}

const JBTypes::timePointMs& Enemy::intersectionTime() const {
    return _intersectionTime;
}

bool Enemy::hasHit() const {
    return _hasHit;
}

float Enemy::size() const {
    return _size;
}

size_t Enemy::length() const {
    return _length;
}

void Enemy::switchOnOff() {
}

JBTypes::vec3f Enemy::init3DPosition(const JBTypes::vec3f& initialPosition) const {
    constexpr float sizeBlock = 1.f;
    constexpr float offset = sizeBlock / 2.f;

    const JBTypes::Dir& currentDir = _direction;
    const JBTypes::vec3f vecDir = JBTypesMethods::directionAsVector(currentDir);

    const JBTypes::vec3f posWorld = { 
        static_cast <float>(initialPosition.at(0)),
        static_cast <float>(initialPosition.at(1)),
        static_cast <float>(initialPosition.at(2))
    };

    const float& radius = _size;
    return {
        posWorld.x + offset + vecDir.x * (offset + radius),
        posWorld.y + offset + vecDir.y * (offset + radius),
        posWorld.z + offset + vecDir.z * (offset + radius)
    };
}

const JBTypes::vec3f& Enemy::initialPosition() const {
    return _initialPosition;
}

const JBTypes::vec3f& Enemy::position() const {
    return _position;
}

const std::array <float, 9>& Enemy::transform() const {
    return _transform;
}

const JBTypes::Dir& Enemy::direction() const {
    return _direction;
}

const JBTypes::Dir& Enemy::movementDirection() const {
    return _direction;
}

SceneElement::DynamicValues <JBTypes::vec3f> Enemy::getDynamicVec3fs() const {
    return {
        { "translation", { _transform.at(0), _transform.at(1), _transform.at(2) }},
        { "scale", { _transform.at(6), _transform.at(7), _transform.at(8) }}
    };
}

SceneElement::GlobalState Enemy::getGlobalState() const {
    return SceneElement::GlobalState::Separate;
}

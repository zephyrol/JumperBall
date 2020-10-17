//
//  Enemy.cpp
//  JumperBallCore
//
//  Created by S Morgenthaler on 08/10/2020.
//

#include "Enemy.h"

Enemy::Enemy(const Block& tieBlock,
             const std::array<unsigned int,3>& initialPosition,
             const JBTypes::Dir& direction,
             const std::array<float,9>& transform):
    _intersectionTime(),
    _hasHit(false),
    _initialPosition(initialPosition),
    _position{ 
               static_cast<float>(initialPosition.at(0)),
               static_cast<float>(initialPosition.at(1)), 
               static_cast<float>(initialPosition.at(2))
             },
    _transform(transform),
    _tieBlock(tieBlock),
    _creationTime(JBTypesMethods::getTimePointMSNow()),
    _direction(direction)
{
}

Enemy::Color Enemy::getColor() const {
    return Enemy::Color::None;
}

const JBTypes::timePointMs& Enemy::creationTime() const {
   return _creationTime; 
}

const JBTypes::timePointMs &Enemy::intersectionTime() const {
    return _intersectionTime;
}

bool Enemy::hasHit() const {
    return _hasHit;
}

const std::array<unsigned int,3>& Enemy::initialPosition() const {
    return _initialPosition;
}

const JBTypes::vec3f& Enemy::position() const {
    return _position; 
}

const std::array<float,9>& Enemy::transform() const {
    return _transform;
}

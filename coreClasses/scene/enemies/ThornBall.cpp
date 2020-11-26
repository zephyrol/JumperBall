//
//  ThornBall.cpp
//  JumperBallCore
//
//  Created by S Morgenthaler on 08/10/2020.
//

#include "ThornBall.h"

ThornBall::ThornBall(const Block& tieBlock, 
             const std::array<unsigned int,3>& initialPosition,
             const JBTypes::Dir& dir,
             const JBTypes::Dir& movementDirection,
             size_t movementLength):
Enemy(tieBlock,
      initialPosition,
      dir,
      thornBallRadius,
      1.f,
      { 0.f,
        0.f,
        0.f,
        0.f,
        0.f,
        0.f,
        1.f,
        1.f,
        1.f }
),
_movementDirection(movementDirection),
_movementLenght(movementLength)
{
}

Enemy::Effect ThornBall::update(const JBTypes::vec3f& entityPosition,
                               float radiusEntity) 
{
    constexpr float movementDuration = 1.f;
    const float timeSinceCreation = 
        JBTypesMethods::getTimeSecondsSinceTimePoint(creationTime());
    
    const JBTypes::vec3f vecDir = 
        JBTypesMethods::directionAsVector(direction());
    const float movementPosition = 
        M_PI -cos(2.f * M_PI * (timeSinceCreation / movementDuration)) / 2.f;
    _transform.at(0) = vecDir.x * movementPosition;
    _transform.at(1) = vecDir.y * movementPosition;
    _transform.at(2) = vecDir.z * movementPosition;
    touchingTest(entityPosition, radiusEntity);
    return _hasHit
        ? Enemy::Effect::Burst
        : Enemy::Effect::Nothing;
}

void ThornBall::touchingTest(const JBTypes::vec3f& entityPosition,
                         float radiusEntity) 
{
    if (JBTypesMethods::distance(entityPosition, _position) <
        (radiusEntity + thornBallRadius)) {
        _intersectionTime = JBTypesMethods::getTimePointMSNow();
        _hasHit = true;
    }
}

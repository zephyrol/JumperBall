//
//  DarkBall.cpp
//  JumperBallCore
//
//  Created by S Morgenthaler on 08/10/2020.
//

#include "DarkBall.h"

DarkBall::DarkBall(const Block &tieBlock,
                   const std::array<unsigned int,3>& initialPosition,
                   const JBTypes::Dir &dir,
                   const JBTypes::Dir &movementDirection,
                   size_t nbOfJumps) : 
Enemy(tieBlock, initialPosition, dir, darkBallRadius, 1.f,
      { static_cast<float>(initialPosition.at(0)), 
        static_cast<float>(initialPosition.at(1)), 
        static_cast<float>(initialPosition.at(2)), 
        0.f,
        0.f,
        0.f,
        1.f,
        1.f,
        1.f }
),
_movementDirection(movementDirection),
_nbOfJumps(nbOfJumps)
{
}

Enemy::EnemyEffect DarkBall::update(const JBTypes::vec3f& entityPosition,
                               float radiusEntity) 
{
   touchingTest(entityPosition,radiusEntity);
   return _hasHit 
    ? Enemy::EnemyEffect::Destroy
    : Enemy::EnemyEffect::Nothing;
}

void DarkBall::touchingTest(const JBTypes::vec3f& entityPosition,
                         float radiusEntity)
{
    if (JBTypesMethods::distance(entityPosition, _position) <
        (radiusEntity + darkBallRadius))
    {
        _intersectionTime = JBTypesMethods::getTimePointMSNow();
        _hasHit = true;
    }
}

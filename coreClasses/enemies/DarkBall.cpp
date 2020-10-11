//
//  DarkBall.cpp
//  JumperBallCore
//
//  Created by S Morgenthaler on 08/10/2020.
//

#include "DarkBall.h"

DarkBall::DarkBall(const Block &tieBlock,
                   const JBTypes::Dir &dir,
                   const JBTypes::Dir &movementDirection,
                   size_t nbOfJumps) : Enemy(tieBlock, dir),
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

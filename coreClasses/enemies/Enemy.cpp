//
//  Enemy.cpp
//  JumperBallCore
//
//  Created by S Morgenthaler on 08/10/2020.
//

#include "Enemy.h"

Enemy::Enemy(const Block& tieBlock, const JBTypes::Dir& direction):
    _intersectionTime(),
    _hasHit(false),
    _position{0.f,0.f,0.f},
    _tieBlock(tieBlock),
    _creationTime(JBTypesMethods::getTimePointMSNow()),
    _direction(direction)

{
}

const JBTypes::timePointMs& Enemy::creationTime() const
{
   return _creationTime; 
}

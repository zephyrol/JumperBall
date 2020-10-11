//
//  ThornBall.cpp
//  JumperBallCore
//
//  Created by S Morgenthaler on 08/10/2020.
//

#include "ThornBall.h"

ThornBall::ThornBall(const Block& tieBlock, 
             const JBTypes::Dir& dir,
             const JBTypes::Dir& movementDirection,
             size_t movementLength):
Enemy(tieBlock,dir),
_movementDirection(movementDirection),
_movementLenght(movementLength)
{
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

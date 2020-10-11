//
//  Laser.cpp
//  JumperBallCore
//
//  Created by S Morgenthaler on 08/10/2020.
//

#include "Laser.h"

Laser::Laser(const Color& color, 
          const Block& tieBlock, 
          const Map& map,
          const JBTypes::Dir& dir, 
          size_t length):
Enemy(tieBlock, dir),
_map(map),
_color(color),
_length(length) {
    
}

Enemy::EnemyEffect Laser::update(const JBTypes::vec3f& entityPosition,
                                 float radiusEntity) {
   touchingTest(entityPosition,radiusEntity);
   return _hasHit 
    ? Enemy::EnemyEffect::Destroy
    : Enemy::EnemyEffect::Nothing;
}

void Laser::touchingTest(const JBTypes::vec3f&,
                         float )
{
    _intersectionTime = JBTypesMethods::getTimePointMSNow();
    _hasHit = true;
}

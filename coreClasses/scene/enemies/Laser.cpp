//
//  Laser.cpp
//  JumperBallCore
//
//  Created by S Morgenthaler on 08/10/2020.
//

#include "Laser.h"

Laser::Laser(const JBTypes::Color& color, 
             const Block& tieBlock, 
             const std::array<unsigned int,3>& initialPosition,
             const JBTypes::Dir& dir,
             size_t length):
Enemy(tieBlock, 
      initialPosition,
      dir,
      static_cast<float>(length),
      static_cast<float>(length),
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
_color(color)
{
}

Enemy::Effect Laser::update(const JBTypes::vec3f& entityPosition,
                                 float radiusEntity) {
    touchingTest(entityPosition,radiusEntity);
    return _hasHit 
    ? Enemy::Effect::Burst
    : Enemy::Effect::Nothing;
}

JBTypes::Color Laser::getColor() const 
{
    return _color;    
}

void Laser::touchingTest(const JBTypes::vec3f&,
                         float )
{
    _intersectionTime = JBTypesMethods::getTimePointMSNow();
    _hasHit = true;
}

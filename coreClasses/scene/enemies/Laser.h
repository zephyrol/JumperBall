//
//  Laser.h
//  JumperBallCore
//
//  Created by S Morgenthaler on 08/10/2020.
//

#ifndef Laser_h
#define Laser_h
#include "Enemy.h"
#include "scene/Map.h"

class Laser : public Enemy {
public:

    Laser(const Color& color, 
          const Block& tieBlock, 
          const std::array<unsigned int,3>& initialPosition,
          const JBTypes::Dir& dir,
          size_t length);

    virtual EnemyEffect update(const JBTypes::vec3f& entityPosition,
                         float radiusEntity) override;
    virtual Enemy::Color getColor() const override;
    virtual ~Laser() = default;

private:
    const Color _color;

    virtual void touchingTest(const JBTypes::vec3f& entityPosition,
                         float radiusEntity) override; 
};

#endif /* Laser_h */

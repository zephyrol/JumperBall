//
//  Laser.h
//  JumperBallCore
//
//  Created by S Morgenthaler on 08/10/2020.
//

#ifndef Laser_h
#define Laser_h
#include "Enemy.h"
#include "Map.h"

class Laser : public Enemy {
public:

    enum class Color { Red, Green, Blue, Yellow };

    Laser(const Color& color, 
          const Block& tieBlock, 
          const Map& map,
          const JBTypes::Dir& dir, 
          size_t length);

    virtual EnemyEffect update(const JBTypes::vec3f& entityPosition,
                         float radiusEntity) override;
    virtual ~Laser() = default;

private:
    const Map& _map;
    const Color _color;
    const size_t _length;

    virtual void touchingTest(const JBTypes::vec3f& entityPosition,
                         float radiusEntity) override; 
};

#endif /* Laser_h */

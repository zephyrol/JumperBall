//
// Laser.h
// JumperBallCore
//
// Created by S Morgenthaler on 08/10/2020.
//

#ifndef Laser_h
#define Laser_h
#include "Enemy.h"
#include "scene/Map.h"

class Laser:public Enemy {
public:

Laser(const JBTypes::Color& color,
      const JBTypes::vec3ui& initialPosition,
      const JBTypes::Dir& dir,
      size_t length);

virtual Effect update(const JBTypes::vec3f& entityPosition,
                      float radiusEntity) override;
virtual JBTypes::Color getColor() const override;
virtual void switchOnOff() override;
virtual ~Laser() = default;

private:
const JBTypes::Color _color;
bool _isActivated;
static constexpr float laserRadius = 0.05f;

virtual void touchingTest(const JBTypes::vec3f& entityPosition,
                          float radiusEntity) override;
};

#endif /* Laser_h */

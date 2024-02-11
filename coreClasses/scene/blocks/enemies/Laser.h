//
// Laser.h
// JumperBallCore
//
// Created by S Morgenthaler on 08/10/2020.
//

#ifndef Laser_h
#define Laser_h

#include "InteractiveEnemy.h"

class Laser : public InteractiveEnemy {
public:

    Laser(
        const JBTypes::Color &color,
        const JBTypes::vec3ui &initialPosition,
        const JBTypes::Dir &dir,
        size_t length,
        const Ball_sptr &ball,
        bool isActivated
    );

    JBTypes::Color getColor() const override;

    vecCstShape_sptr getShapes() const override;

    void switchOnOff() override;

    ~Laser() override = default;

private:
    const JBTypes::Color _color;
    static constexpr float laserRadius = 0.05f;

    bool touchingTest() const override;
};

#endif /* Laser_h */

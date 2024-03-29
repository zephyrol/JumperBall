//
// DarkBall.h
// JumperBallCore
//
// Created by S Morgenthaler on 08/10/2020.
//

#ifndef DarkBall_h
#define DarkBall_h

#include "scene/ClassicalMechanics.h"
#include "InteractiveEnemy.h"

class DarkBall : public InteractiveEnemy {
public:
    DarkBall(
        const JBTypes::vec3ui &initialPosition,
        const JBTypes::Dir &dir,
        const JBTypes::Dir &movementDirection,
        size_t nbOfJumps,
        const Ball_sptr &ball
    );

    void update() override;

    const JBTypes::Dir &movementDirection() const;

    ~DarkBall() override = default;

    static constexpr float darkBallRadius = 0.2f;

    vecCstShape_sptr getShapes() const override;

private:
    const CstChronometer_sptr _chronometer;
    const JBTypes::Dir _movementDirection;

    bool touchingTest() const override;

    static const ClassicalMechanics darkBallClassicalMechanics;
};

#endif /* DarkBall_h */

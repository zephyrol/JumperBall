// ThornBall.h
// JumperBallCore
//
// Created by S Morgenthaler on 08/10/2020.
//

#ifndef ThornBall_h
#define ThornBall_h

#include "InteractiveEnemy.h"

class ThornBall : public InteractiveEnemy {
public:
    ThornBall(
        const JBTypes::vec3ui &initialPosition,
        const JBTypes::Dir &dir,
        const JBTypes::Dir &movementDirection,
        size_t movementLength,
        const Ball_sptr &ball
    );

    void update() override;

    const JBTypes::Dir &movementDirection() const;

    vecCstShape_sptr getShapes() const override;

    ~ThornBall() override = default;

    static constexpr float thornBallRadius = 0.1f;

private:
    const CstChronometer_sptr _chronometer;
    const JBTypes::Dir _movementDirection;

    bool touchingTest() const override;
};

#endif /* ThornBall_h */

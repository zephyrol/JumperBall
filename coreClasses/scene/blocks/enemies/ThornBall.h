// ThornBall.h
// JumperBallCore
//
// Created by S Morgenthaler on 08/10/2020.
//

#ifndef ThornBall_h
#define ThornBall_h
#include "InteractiveEnemy.h"

class ThornBall:public InteractiveEnemy {
public:
ThornBall(const JBTypes::vec3ui& initialPosition,
          const JBTypes::Dir& dir,
          const JBTypes::Dir& movementDirection,
          size_t movementLength,
          const Ball_sptr &ball
          );

Effect update() override;

const JBTypes::Dir& movementDirection() const ;
vecCstShape_sptr getShapes() const override;
virtual ~ThornBall() = default;
static constexpr float thornBallRadius = 0.1f;

private:
const JBTypes::Dir _movementDirection;

void touchingTest() override;
};

#endif /* ThornBall_h */

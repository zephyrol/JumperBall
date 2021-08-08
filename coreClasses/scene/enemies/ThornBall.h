// ThornBall.h
// JumperBallCore
//
// Created by S Morgenthaler on 08/10/2020.
//

#ifndef ThornBall_h
#define ThornBall_h
#include "Enemy.h"

class ThornBall:public Enemy {
public:
ThornBall(const JBTypes::vec3ui& initialPosition,
          const JBTypes::Dir& dir,
          const JBTypes::Dir& movementDirection,
          size_t movementLength,
          const Ball_sptr &ball
          );

virtual Effect update(const JBTypes::vec3f& boundingSpherePosition,
                      float boundingSphereRadius) override;

const JBTypes::Dir& movementDirection() const ;
virtual ~ThornBall() = default;
static constexpr float thornBallRadius = 0.1f;

private:
const JBTypes::Dir _movementDirection;

virtual void touchingTest(const JBTypes::vec3f& boundingSpherePosition,
                          float boundingSphereRadius) override;
};

#endif /* ThornBall_h */

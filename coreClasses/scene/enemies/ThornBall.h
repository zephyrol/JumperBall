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
ThornBall(const Block& tieBlock,
          const JBTypes::vec3ui& initialPosition,
          const JBTypes::Dir& dir,
          const JBTypes::Dir& movementDirection,
          size_t movementLength);
virtual Effect update(const JBTypes::vec3f& entityPosition,
                      float radiusEntity) override;
virtual const JBTypes::Dir& movementDirection() const override;
virtual ~ThornBall() = default;
static constexpr float thornBallRadius = 0.1f;

private:
const JBTypes::Dir _movementDirection;

virtual void touchingTest(const JBTypes::vec3f& entityPosition,
                          float radiusEntity) override;
};

#endif /* ThornBall_h */

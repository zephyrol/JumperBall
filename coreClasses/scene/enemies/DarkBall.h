//
// DarkBall.h
// JumperBallCore
//
// Created by S Morgenthaler on 08/10/2020.
//

#ifndef DarkBall_h
#define DarkBall_h
#include "scene/ClassicalMechanics.h"
#include "Enemy.h"

class DarkBall:public Enemy {
public:
DarkBall(const JBTypes::vec3ui& initialPosition,
         const JBTypes::Dir& dir,
         const JBTypes::Dir& movementDirection,
         size_t nbOfJumps);
virtual Effect update(const JBTypes::vec3f& entityPosition,
                      float radiusEntity) override;
virtual const JBTypes::Dir& movementDirection() const override;
virtual ~DarkBall() = default;
static constexpr float darkBallRadius = 0.2f;

private:
const JBTypes::Dir _movementDirection;
virtual void touchingTest(const JBTypes::vec3f& entityPosition,
                          float radiusEntity) override;
static const ClassicalMechanics darkBallClassicalMechanics;
};

#endif /* DarkBall_h */

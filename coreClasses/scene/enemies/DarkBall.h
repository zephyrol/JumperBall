//
//  DarkBall.h
//  JumperBallCore
//
//  Created by S Morgenthaler on 08/10/2020.
//

#ifndef DarkBall_h
#define DarkBall_h
#include "Enemy.h"

class DarkBall : public Enemy{
public:
    DarkBall(const Block& tieBlock, 
             const std::array<unsigned int,3>& initialPosition,
             const JBTypes::Dir& dir,
             const JBTypes::Dir& movementDirection,
             size_t nbOfJumps);
    virtual EnemyEffect update(const JBTypes::vec3f& entityPosition,
                               float radiusEntity) override;
    virtual ~DarkBall() = default;
    static constexpr float darkBallRadius = 0.2f;

private:
    const JBTypes::Dir _movementDirection;
    const size_t _nbOfJumps;
    virtual void touchingTest(const JBTypes::vec3f& entityPosition,
                         float radiusEntity) override; 
};

#endif /* DarkBall_h */

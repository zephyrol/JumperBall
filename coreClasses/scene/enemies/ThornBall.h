//  ThornBall.h
//  JumperBallCore
//
//  Created by S Morgenthaler on 08/10/2020.
//

#ifndef ThornBall_h
#define ThornBall_h
#include "Enemy.h"

class ThornBall : public Enemy{
public:
    ThornBall(const Block& tieBlock, 
              const std::array<unsigned int,3>& initialPosition,
              const JBTypes::Dir& dir,
              const JBTypes::Dir& movementDirection,
              size_t movementLength);
    virtual ~ThornBall() = default;
    
private:
    const JBTypes::Dir _movementDirection;
    const size_t _movementLenght;
    static constexpr float thornBallRadius = 0.2f;

    virtual void touchingTest(const JBTypes::vec3f& entityPosition,
                         float radiusEntity) override; 
};

#endif /* ThornBall_h */
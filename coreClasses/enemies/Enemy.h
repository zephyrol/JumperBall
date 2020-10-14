//
//  Enemy.h
//  JumperBallCore
//
//  Created by S Morgenthaler on 08/10/2020.
//
#ifndef Enemy_hpp
#define Enemy_hpp
#include "../Types.h"
#include "../blocks/Block.h"

class Enemy {
public:

    enum class EnemyEffect { Nothing, Destroy };

    Enemy(const Block& tieBlock, const JBTypes::Dir& direction);

    virtual EnemyEffect update(const JBTypes::vec3f& entityPosition,
                               float radiusEntity) = 0;
    const JBTypes::timePointMs& creationTime() const;
    const JBTypes::timePointMs& intersectionTime() const;
    const JBTypes::vec3f position() const;
    bool hasHit() const;
    virtual ~Enemy() = default;

protected :
    JBTypes::timePointMs _intersectionTime;
    bool _hasHit;
    JBTypes::vec3f _position;

private:
    const Block& _tieBlock;
    const JBTypes::timePointMs _creationTime;
    const JBTypes::Dir _direction;

    virtual void touchingTest(const JBTypes::vec3f& entityPosition,
                              float radiusEntity) = 0;
};

#endif /* Enemy_h */

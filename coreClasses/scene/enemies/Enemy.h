//
//  Enemy.h
//  JumperBallCore
//
//  Created by S Morgenthaler on 08/10/2020.
//
#ifndef Enemy_hpp
#define Enemy_hpp
#include "scene/blocks/Block.h"

class Enemy {
public:

    enum class EnemyEffect { Nothing, Destroy };

    Enemy(const Block& tieBlock,
          const std::array<unsigned int,3>& initialPosition,
          const JBTypes::Dir& direction,
          const float size,
          const std::array<float,9>& transform);

    virtual EnemyEffect update(const JBTypes::vec3f& entityPosition,
                               float radiusEntity) = 0;
    virtual JBTypes::Color getColor() const;
    const JBTypes::timePointMs& creationTime() const;
    const JBTypes::timePointMs& intersectionTime() const;
    const std::array<unsigned int,3>& initialPosition() const;
    const JBTypes::vec3f& position() const;
    const std::array<float,9>& transform() const;
    const JBTypes::Dir& direction() const;
    const JBTypes::Dir& movementDirection() const;
    bool hasHit() const;
    float size() const;
    virtual ~Enemy() = default;

protected :
    JBTypes::timePointMs _intersectionTime;
    bool _hasHit;
    const std::array<unsigned int, 3> _initialPosition;
    JBTypes::vec3f _position;
    std::array<float,9> _transform;

private:
    const Block& _tieBlock;
    const JBTypes::timePointMs _creationTime;
    const JBTypes::Dir _direction;
    const float _size;

    virtual void touchingTest(const JBTypes::vec3f& entityPosition,
                              float radiusEntity) = 0;
};

#endif /* Enemy_h */

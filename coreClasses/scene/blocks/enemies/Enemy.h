//
// Enemy.h
// JumperBallCore
//
// Created by S Morgenthaler on 08/10/2020.
//
#ifndef Enemy_hpp
#define Enemy_hpp

#include <scene/Shape.h>
#include "scene/SceneElement.h"

class Enemy;

using Enemy_sptr = std::shared_ptr<Enemy>;
using CstEnemy_sptr = std::shared_ptr<const Enemy>;
using vecCstEnemy_sptr = std::vector<CstEnemy_sptr>;
using vecEnemy_sptr = std::vector<Enemy_sptr>;
using Enemy_uptr = std::unique_ptr<Enemy>;

class Enemy : public SceneElement {
public:
    Enemy(
        const glm::u32vec3 &initialPosition,
        const JBTypes::Dir &direction,
        float size,
        size_t length,
        bool isActivated
    );

    virtual void update();

    virtual JBTypes::Color getColor() const;

    const glm::vec3 &position() const;

    const JBTypes::Dir &direction() const;

    float size() const;

    size_t length() const;

    Displayable::DynamicValues<glm::vec3> getDynamicVec3fValues() const override;

    std::string getDynamicGroupHash() const override;

    virtual bool touchingTest() const = 0;

    virtual void switchOnOff();

private:
    const JBTypes::Dir _direction;
    const float _size;
    const size_t _length;

protected:
    const glm::vec3 _initialPosition;
    glm::vec3 _position;
    bool _isActivated;
    glm::vec3 _scale;

    static const glm::vec3 scaleActivated;
    static const glm::vec3 scaleDisable;

private:
    glm::vec3 init3DPosition(const glm::u32vec3 &initialPosition) const;

};

#endif /* Enemy_hpp */

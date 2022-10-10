//
// Enemy.h
// JumperBallCore
//
// Created by S Morgenthaler on 08/10/2020.
//
#ifndef Enemy_hpp
#define Enemy_hpp

#include <scene/Shape.h>
#include "scene/Displayable.h"

class Enemy;

using Enemy_sptr = std::shared_ptr<Enemy>;
using CstEnemy_sptr = std::shared_ptr<const Enemy>;
using vecCstEnemy_sptr = std::vector<CstEnemy_sptr>;
using vecEnemy_sptr = std::vector<Enemy_sptr>;
using Enemy_uptr = std::unique_ptr<Enemy>;

class Enemy : public Displayable {
public:
    enum class Effect {
        Nothing, Burst
    };

    Enemy(
        const JBTypes::vec3ui &initialPosition,
        const JBTypes::Dir &direction,
        float size,
        size_t length,
        bool isActivated
    );

    virtual void update();

    virtual JBTypes::Color getColor() const;

    const JBTypes::vec3f &position() const;

    const JBTypes::Dir &direction() const;

    float size() const;

    size_t length() const;

    Displayable::DynamicValues<JBTypes::vec3f> getDynamicVec3fs() const override;

    Displayable::GlobalState getGlobalState() const override;

    virtual vecCstShape_sptr getShapes() const = 0;

    virtual bool touchingTest() const = 0;

    virtual void switchOnOff();

    virtual ~Enemy() = default;

private:
    const JBTypes::Dir _direction;
    const float _size;
    const size_t _length;

protected:
    const JBTypes::vec3f _initialPosition;
    JBTypes::vec3f _position;
    bool _isActivated;
    JBTypes::vec3f _scale;

    static const JBTypes::vec3f scaleActivated;
    static const JBTypes::vec3f scaleDisable;

private:
    JBTypes::vec3f init3DPosition(const JBTypes::vec3ui &initialPosition) const;

};

#endif /* Enemy_hpp */

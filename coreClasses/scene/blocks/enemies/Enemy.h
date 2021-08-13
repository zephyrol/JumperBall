//
// Enemy.h
// JumperBallCore
//
// Created by S Morgenthaler on 08/10/2020.
//
#ifndef Enemy_hpp
#define Enemy_hpp

#include <scene/Ball.h>
#include "scene/SceneElement.h"


class Enemy;
using Enemy_sptr = std::shared_ptr <Enemy>;
using CstEnemy_sptr = std::shared_ptr <const Enemy>;
using vecCstEnemy_sptr = std::vector <CstEnemy_sptr>;
using vecEnemy_sptr = std::vector <Enemy_sptr>;
using Enemy_uptr = std::unique_ptr <Enemy>;

class Enemy : public SceneElement{
public:
    enum class Effect { Nothing, Burst };

Enemy(const JBTypes::vec3ui& initialPosition,
      const JBTypes::Dir& direction,
      float size,
      size_t length,
      const std::array <float, 9>& transform,
      const Ball_sptr &ball
      );

virtual Effect update(const JBTypes::vec3f& boundingSpherePosition, float boundingSphereRadius) = 0;
virtual JBTypes::Color getColor() const;
const JBTypes::timePointMs& creationTime() const;
const JBTypes::timePointMs& intersectionTime() const;
const JBTypes::vec3f& position() const;
const std::array <float, 9>& transform() const;
const JBTypes::Dir& direction() const;
bool hasHit() const;
float size() const;
size_t length() const;

SceneElement::DynamicValues <JBTypes::vec3f> getDynamicVec3fs() const override;

SceneElement::GlobalState getGlobalState() const override;

virtual void switchOnOff();
virtual ~Enemy() = default;

private:
const JBTypes::timePointMs _creationTime;
const JBTypes::Dir _direction;
const float _size;
const size_t _length;
const Ball_sptr _ball;

protected:
JBTypes::timePointMs _intersectionTime;
bool _hasHit;
const JBTypes::vec3f _initialPosition;
JBTypes::vec3f _position;
std::array <float, 9> _transform;

private:
JBTypes::vec3f init3DPosition(const JBTypes::vec3ui& initialPosition) const;

virtual void touchingTest(const JBTypes::vec3f& boundingSpherePosition, float boundingSphereRadius) = 0;
};

#endif /* Enemy_h */

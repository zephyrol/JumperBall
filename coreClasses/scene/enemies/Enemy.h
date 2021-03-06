//
// Enemy.h
// JumperBallCore
//
// Created by S Morgenthaler on 08/10/2020.
//
#ifndef Enemy_hpp
#define Enemy_hpp
#include "scene/blocks/Block.h"
#include "scene/SceneElement.h"

class Enemy : public SceneElement{
public:

enum class Effect { Nothing, Burst };

Enemy(const Block& tieBlock,
      const JBTypes::vec3ui& initialPosition,
      const JBTypes::Dir& direction,
      float size,
      size_t length,
      const std::array <float, 9>& transform);

virtual Effect update(const JBTypes::vec3f& entityPosition,
                      float radiusEntity) = 0;
virtual JBTypes::Color getColor() const;
const JBTypes::timePointMs& creationTime() const;
const JBTypes::timePointMs& intersectionTime() const;
const JBTypes::vec3f& initialPosition() const;
const JBTypes::vec3f& position() const;
const std::array <float, 9>& transform() const;
const JBTypes::Dir& direction() const;
virtual const JBTypes::Dir& movementDirection() const;
bool hasHit() const;
float size() const;
size_t length() const;

virtual SceneElement::DynamicValues <JBTypes::vec3f> getDynamicVec3fs() const override;

SceneElement::GlobalState getGlobalState() const override;

virtual void switchOnOff();
virtual ~Enemy() = default;

private:
const Block& _tieBlock;
const JBTypes::timePointMs _creationTime;
const JBTypes::Dir _direction;
const float _size;
const size_t _length;

protected:
JBTypes::timePointMs _intersectionTime;
bool _hasHit;
const JBTypes::vec3f _initialPosition;
JBTypes::vec3f _position;
std::array <float, 9> _transform;


private:
JBTypes::vec3f initPosition(const JBTypes::vec3ui& position)
const;

virtual void touchingTest(const JBTypes::vec3f& entityPosition,
                          float radiusEntity) = 0;
};

#endif /* Enemy_h */

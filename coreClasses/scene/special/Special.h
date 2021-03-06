/*
 * File: Special.h
 * Author: Morgenthaler S
 *
 * Created on 7 novembre 2020, 10:42
 */
#ifndef __SPECIAL_H__
#define __SPECIAL_H__
#include "scene/blocks/Block.h"
#include "scene/SceneElement.h"

class Special: public SceneElement {
public:
enum class SpecialEffect { Switch,
                           Teleportation };

Special(
    const JBTypes::Color& color,
    const Block& tieBlock,
    const JBTypes::Dir& dir,
    const JBTypes::vec3ui& position,
    bool isActivated = true
    );

virtual ~Special() = default;

const JBTypes::Color& getColor() const;
float getTimeSinceCreation() const;
const JBTypes::Dir& direction() const;
const JBTypes::vec3f& position3D() const;
const JBTypes::vec3ui& position() const;
bool isActivated() const;
virtual bool isAnimated() const;
virtual SpecialEffect getEffect() const = 0;

void switchOnOff();

SceneElement::StaticValues <float> getStaticFloatValues() const override;
SceneElement::StaticValues <JBTypes::vec3f> getStaticVec3fValues() const override;

SceneElement::DynamicValues <float> getDynamicFloats() const override;

SceneElement::GlobalState getGlobalState() const override;

private:
const Block& _tieBlock;
const JBTypes::timePointMs _creationTime;
const JBTypes::Dir _direction;
const JBTypes::Color _color;
const std::string _colorAttributeName;
const JBTypes::vec3ui _position;
const JBTypes::vec3f _position3D;
bool _isActivated;

JBTypes::vec3f initPosition(const JBTypes::vec3ui& position) const;
};


#endif // __SPECIAL_H__

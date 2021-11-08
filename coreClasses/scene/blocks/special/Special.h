/*
 * File: Special.h
 * Author: Morgenthaler S
 *
 * Created on 7 novembre 2020, 10:42
 */
#ifndef __SPECIAL_H__
#define __SPECIAL_H__
#include <scene/Shape.h>
#include "scene/SceneElement.h"

class Special;
using Special_sptr = std::shared_ptr <Special>;
using CstSpecial_sptr = std::shared_ptr <const Special>;
using vecCstSpecial_sptr = std::vector <CstSpecial_sptr>;
using vecSpecial_sptr = std::vector <Special_sptr>;
using Special_uptr = std::unique_ptr <Special>;
class Special: public SceneElement {
public:

Special(
    const JBTypes::Color& color,
    const JBTypes::Dir& dir,
    const JBTypes::vec3ui& position,
    bool isActivated = true
    );

const JBTypes::Color& getColor() const;
float getTimeSinceCreation() const;
const JBTypes::Dir& direction() const;
const JBTypes::vec3f& position3D() const;
const JBTypes::vec3ui& position() const;
bool isActivated() const;
virtual bool isAnimated() const;
virtual void applySpecialEffect() = 0;

void switchOnOff();

virtual vecCstShape_sptr getShapes() const = 0;
SceneElement::StaticValues <float> getStaticFloatValues() const override;
SceneElement::StaticValues <JBTypes::vec3f> getStaticVec3fValues() const override;

SceneElement::DynamicValues <float> getDynamicFloats() const override;

SceneElement::GlobalState getGlobalState() const override;

private:
const JBTypes::timePointMs _creationTime;
const JBTypes::Dir _direction;
const JBTypes::Color _color;
const std::string _colorAttributeName;
const JBTypes::vec3ui _position;
const JBTypes::vec3f _position3D;
bool _isActivated;

JBTypes::vec3f init3DPosition() const;
};


#endif // __SPECIAL_H__

/*
 * File: StarState.h
 * Author: Morgenthaler S
 *
 * Created on 19 septembre 2020, 07h50
 */
/*#ifndef STARSTATE_H
#define STARSTATE_H
#include "ObjectState.h"
#include "Star.h"


class StarState:public SceneElement {
public:
StarState(const Star& star);

SceneElement::SceneElementState update() override;
StarState::DynamicValues <float> getDynamicFloats() const override;
StarState::DynamicValues <JBTypes::vec3f> getDynamicVec3fs() const override;
StarState::DynamicValues <JBTypes::Quaternion> getDynamicQuaternions() const override;

float getEnvSize() const;
const JBTypes::vec3f& getRotationCenter() const;
const JBTypes::vec3f& getPosition() const;
const JBTypes::vec3f& getLightDirection() const;

static const std::string lightDirectionName;

private:
const Star& _star;

JBTypes::Quaternion _rotation;
JBTypes::vec3f _position;
JBTypes::vec3f _lightDirection;

};


#endif */// STARSTATE_H

/*
 * File: SpecialState.h
 * Author: Morgenthaler S
 *
 * Created on 14 novembre 2020, 18h09
 */
#ifndef SPECIALSTATE_H
#define SPECIALSTATE_H
#include "Special.h"
#include <scene/Map.h>
#include <scene/ObjectState.h>

class SpecialState:public ObjectState {
public:
SpecialState(const Special& special);
virtual ObjectState::GlobalState update() override;

ObjectState::StaticValues <float> getStaticFloatValues() const override;
ObjectState::StaticValues <JBTypes::vec3f> getStaticVec3fValues() const override;

ObjectState::DynamicValues <float> getDynamicFloats() const override;

private:
const Special& _special;
const JBTypes::vec3ui& _position;
const float _direction;
const float _color;
const std::string _colorAttributeName;
const float _isAnimated;
float _timeSinceCreation;
bool _isActivated;
};

#endif // SPECIALSTATE_H

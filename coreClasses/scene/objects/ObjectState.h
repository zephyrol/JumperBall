/*
 * File: ObjectState.h
 * Author: Morgenthaler S
 *
 * Created on 19 septembre 2020, 07h50
 */
#ifndef OBJECTSTATE_H
#define OBJECTSTATE_H
#include <scene/objects/Object.h>
#include <scene/State.h>


class ObjectState:public State {
public:
ObjectState(const Object& object);

State::StaticValues <float> getStaticFloatValues() const override;
State::StaticValues <JBTypes::vec3f> getStaticVec3fValues() const override;

State::DynamicValues <float> getDynamicFloats() const override;

State::GlobalState update() override;

private:
const Object& _object;
const JBTypes::vec3ui& _position;
const float _direction;
float _timeSinceCreation;
float _timeSinceObtaining;

};

#endif // OBJECTSTATE_H

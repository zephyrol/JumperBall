/*
 * File: StarState.h
 * Author: Morgenthaler S
 *
 * Created on 19 septembre 2020, 07h50
 */
#ifndef STARSTATE_H
#define STARSTATE_H
#include "State.h"
#include "Star.h"


class StarState:public State {
public:
StarState(const Star& star);

State::GlobalState update() override;
const JBTypes::vec3f& lightDirection() const;
const JBTypes::vec3f& position() const;
const JBTypes::vec3f& rotationCenter() const;
float envSize() const;
std::map <std::string, float> getDynamicFloats() const override;
std::map <std::string, JBTypes::vec3f> getDynamicVec3fs() const override;
std::map <std::string, JBTypes::Quaternion> getDynamicQuaternions() const override;

private:
const Star& _star;

JBTypes::Quaternion _rotation;
JBTypes::vec3f _lightDirection;
JBTypes::vec3f _position;
};


#endif // STARSTATE_H

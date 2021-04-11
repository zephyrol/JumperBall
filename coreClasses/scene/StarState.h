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
StarState::DynamicValues <float> getDynamicFloats() const override;
StarState::DynamicValues <JBTypes::vec3f> getDynamicVec3fs() const override;
StarState::DynamicValues <JBTypes::Quaternion> getDynamicQuaternions() const override;

static const std::string lightDirectionName;
static const std::string positionName;
static const std::string rotationCenterName;
static const std::string envSizeName;

private:
const Star& _star;

JBTypes::Quaternion _rotation;
};


#endif // STARSTATE_H

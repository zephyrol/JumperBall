/*
 * File: BallState.cpp
 * Author: Morgenthaler S
 *
 * Created on 19 septembre 2020, 07h50
 */
#ifndef BALLSTATE_H
#define BALLSTATE_H
#include <scene/Ball.h>
#include "State.h"


class BallState:public ObjectState {
public:
BallState(const Ball& ball);

virtual ObjectState::GlobalState update() override;
float teleportationCoeff() const;
const JBTypes::Color& teleportationColor() const;

ObjectState::DynamicValues <float> getDynamicFloats() const override;
ObjectState::DynamicValues <JBTypes::vec3f> getDynamicVec3fs() const override;
ObjectState::DynamicValues <JBTypes::Quaternion> getDynamicQuaternions() const override;

private:
const Ball& _ball;
JBTypes::Quaternion _coveredRotation;
JBTypes::vec3f _position;
float _burnCoefficient;
JBTypes::vec3f _currentSideAsVector;
float _stateOfLifeStatus;
float _radius;
float _crushingCoeff;
float _teleportationCoeff;
JBTypes::Color _teleportationColor;
float _timeSecondsSinceStateOfLife;

float getStateOfLifeStatus() const;

};

#endif // BALLSTATE_H

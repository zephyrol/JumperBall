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


class BallState:public State {
public:
BallState(const Ball& ball);

virtual State::GlobalState update() override;
const JBTypes::vec3f& get3DPosition() const noexcept;
float burnCoefficient() const;
const JBTypes::vec3f& currentSideAsVector() const;
float getTimeSecondsSinceAction() const;
float getTimeSecondsSinceStateOfLife() const;
Ball::State state() const;
float getRadius() const;
const JBTypes::vec3f& lookTowardsAsVector() const;
float crushingCoeff() const noexcept;
float teleportationCoeff() const;
float timeToGetDestination() const;
const JBTypes::Color& teleportationColor() const;
const JBTypes::vec3f& nextLook() const;


std::map <std::string, float> getDynamicFloats() const override;
std::map <std::string, JBTypes::vec3f> getDynamicVec3fs() const override;
std::map <std::string, JBTypes::Quaternion> getDynamicQuaternions() const override;

private:
const Ball& _ball;
JBTypes::Quaternion _coveredRotation;
JBTypes::vec3f _position;
float _burnCoefficient;
JBTypes::vec3f _currentSideAsVector;
Ball::State _state;
float _stateOfLifeStatus;
float _radius;
float _timeToGetDestination;
JBTypes::vec3f _lookTowardsAsVector;
float _crushingCoeff;
float _teleportationCoeff;
JBTypes::Color _teleportationColor;
float _timeSecondsSinceAction;
float _timeSecondsSinceStateOfLife;
JBTypes::vec3f _nextLook;

float getStateOfLifeStatus() const;

};

#endif // BALLSTATE_H

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

virtual void update() override;
const JBTypes::vec3f& get3DPosition() const noexcept;
float burnCoefficient() const;
const JBTypes::vec3f& currentSideAsVector() const;
const JBTypes::timePointMs& getTimeStateOfLife() const;
const JBTypes::timePointMs& getTimeAction() const noexcept;
Ball::State state() const;
Ball::StateOfLife stateOfLife() const;
float getRadius() const;
float jumpingPosX() const;
float fallingPosX() const;
const JBTypes::vec3f& lookTowardsAsVector() const;
const JBTypes::vec3f& currentMovementRotation() const;
const std::vector <JBTypes::Dir>&
coveredRotation() const noexcept;
float crushingCoeff() const noexcept;
float teleportationCoeff() const;
const JBTypes::Color& teleportationColor() const;

private:
const Ball& _ball;
const std::vector <JBTypes::Dir>&
_coveredRotation;
JBTypes::vec3f _position;
float _burnCoefficient;
JBTypes::vec3f _currentSideAsVector;
JBTypes::timePointMs _timeAction;
JBTypes::timePointMs _timeStateOfLife;
Ball::State _state;
Ball::StateOfLife _stateOfLife;
float _radius;
JBTypes::vec3f _lookTowardsAsVector;
JBTypes::vec3f _currentMovementRotation;
float _crushingCoeff;
float _teleportationCoeff;
JBTypes::Color _teleportationColor;
};

#endif // BALLSTATE_H

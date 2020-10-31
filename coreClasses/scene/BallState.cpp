/*
 * File:   BallState.h
 * Author: Morgenthaler S
 *
 * Created on 19 septembre 2020, 07h50
 */
#include "BallState.h"

BallState::BallState(const Ball& ball):
    _ball(ball),
    _coveredRotation(ball.getCurrentCoveredRotation()),
    _position(ball.get3DPosition()),
    _burnCoefficient(ball.burnCoefficient()),
    _currentSideAsVector(ball.currentSideAsVector()),
    _timeAction(ball.getTimeActionMs()),
    _timeStateOfLife(ball.getTimeStateOfLifeMs()),
    _state(ball.state()),
    _stateOfLife(ball.stateOfLife()),
    _radius(ball.getRadius()),
    _lookTowardsAsVector(ball.lookTowardsAsVector()),
    _currentMovementRotation(ball.movementRotation()),
    _crushingCoeff(ball.getCrushingCoefficient())
{
}

void BallState::update()
{
    _position = _ball.get3DPosition();
    _burnCoefficient = _ball.burnCoefficient();
    _currentSideAsVector = _ball.currentSideAsVector();
    _timeAction = _ball.getTimeActionMs();
    _timeStateOfLife = _ball.getTimeStateOfLifeMs();
    _state = _ball.state();
    _stateOfLife = _ball.stateOfLife();
    _radius = _ball.getRadius();
    _lookTowardsAsVector = _ball.lookTowardsAsVector();
    _currentMovementRotation = _ball.movementRotation();
    _crushingCoeff = _ball.getCrushingCoefficient();
}

const JBTypes::vec3f &BallState::get3DPosition() const noexcept {
    return _position;
}

float BallState::burnCoefficient() const {
    return _burnCoefficient;
}

const JBTypes::vec3f &BallState::currentSideAsVector() const {
    return _currentSideAsVector;
}

const JBTypes::timePointMs& BallState::getTimeStateOfLife() const {
    return _timeStateOfLife;
}

const JBTypes::timePointMs& BallState::getTimeAction() const noexcept {
    return _timeAction;
}

Ball::State BallState::state() const {
    return _state;
}

Ball::StateOfLife BallState::stateOfLife() const {
    return _stateOfLife;
}

float BallState::getRadius() const {
    return _radius;
}

const JBTypes::vec3f &BallState::lookTowardsAsVector() const {
    return _lookTowardsAsVector;
}

const JBTypes::vec3f& BallState::currentMovementRotation() const {
    return _currentMovementRotation;
}

const std::vector<JBTypes::Dir>& BallState::coveredRotation() const noexcept {
    return _coveredRotation;
}

float BallState::crushingCoeff() const noexcept {
    return _crushingCoeff; 
}

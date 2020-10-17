/*
 * File:   BallState.h
 * Author: Morgenthaler S
 *
 * Created on 19 septembre 2020, 07h50
 */
#include "BallState.h"

BallState::BallState(const Ball& ball):
    _ball(ball),
    _position(ball.get3DPosition()),
    _burnCoefficient(ball.burnCoefficient()),
    _currentSideAsVector(ball.currentSideAsVector()),
    _timeAction(ball.getTimeActionMs()),
    _timeStateOfLife(ball.getTimeStateOfLifeMs()),
    _state(ball.state()),
    _stateOfLife(ball.stateOfLife()),
    _radius(ball.getRadius()),
    _jumpingPosX(ball.getMechanicsJumping().getPosition(
                    ball.getTimeSecondsSinceAction()
                    ).x ),
    _fallingPosX(ball.getMechanicsFalling().getPosition(
                    ball.getTimeSecondsSinceAction()
                    ).x ),
    _lookTowardsAsVector(ball.lookTowardsAsVector())
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
    _jumpingPosX = _ball.getMechanicsJumping().getPosition(
                    _ball.getTimeSecondsSinceAction()
                    ).x ;
    _fallingPosX = _ball.getMechanicsFalling().getPosition(
                    _ball.getTimeSecondsSinceAction()
                    ).x ;
    _lookTowardsAsVector = _ball.lookTowardsAsVector();
}

const JBTypes::vec3f &BallState::get3DPosition() const noexcept
{
    return _position;
}

float BallState::burnCoefficient() const
{
    return _burnCoefficient;
}

const JBTypes::vec3f &BallState::currentSideAsVector() const
{
    return _currentSideAsVector;
}

const JBTypes::timePointMs& BallState::getTimeStateOfLife() const
{
    return _timeStateOfLife;
}

const JBTypes::timePointMs& BallState::getTimeAction() const noexcept
{
    return _timeAction;
}

Ball::State BallState::state() const
{
    return _state;
}

Ball::StateOfLife BallState::stateOfLife() const
{
    return _stateOfLife;
}


float BallState::getRadius() const
{
    return _radius;
}

float BallState::jumpingPosX() const
{
    return _jumpingPosX;
}

float BallState::fallingPosX() const
{
    return _fallingPosX;
}

const JBTypes::vec3f &BallState::lookTowardsAsVector() const
{
    return _lookTowardsAsVector;
}


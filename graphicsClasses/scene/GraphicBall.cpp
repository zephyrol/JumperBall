/*
 * File:   GraphicBall.h
 * Author: Morgenthaler S
 *
 * Created on 19 septembre 2020, 07h50
 */
#include "GraphicBall.h"

GraphicBall::GraphicBall(const Ball& ball):
    _ball(ball),
    _position(ball.get3DPosition()),
    _burnCoefficient(ball.burnCoefficient()),
    _currentSideAsVector(ball.currentSideAsVector()),
    _timeSecondsSinceAction(ball.getTimeSecondsSinceAction()),
    _timeSecondsSinceStateOfLife(ball.getTimeSecondsSinceStateOfLife()),
    _state(ball.state()),
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

void GraphicBall::update()
{
    _position = _ball.get3DPosition();
    _burnCoefficient = _ball.burnCoefficient();
    _currentSideAsVector = _ball.currentSideAsVector();
    _timeSecondsSinceAction = _ball.getTimeSecondsSinceAction();
    _timeSecondsSinceStateOfLife = _ball.getTimeSecondsSinceStateOfLife();
    _state = _ball.state();
    _radius = _ball.getRadius();
    _jumpingPosX = _ball.getMechanicsJumping().getPosition(
                    _ball.getTimeSecondsSinceAction()
                    ).x ;
    _fallingPosX = _ball.getMechanicsFalling().getPosition(
                    _ball.getTimeSecondsSinceAction()
                    ).x ;
    _lookTowardsAsVector = _ball.lookTowardsAsVector();
}

const JBTypes::vec3f &GraphicBall::get3DPosition() const noexcept
{
    return _position;
}

float GraphicBall::burnCoefficient() const
{
    return _burnCoefficient;
}

const JBTypes::vec3f &GraphicBall::currentSideAsVector() const
{
    return _currentSideAsVector;
}

float GraphicBall::getTimeSecondsSinceStateOfLife() const
{
    return _timeSecondsSinceStateOfLife;
}

float GraphicBall::getTimeSecondsSinceAction() const noexcept
{
    return _timeSecondsSinceAction;
}

Ball::State GraphicBall::state() const
{
    return _state;
}

Ball::StateOfLife GraphicBall::stateOfLife() const
{
    return _stateOfLife;
}


float GraphicBall::getRadius() const
{
    return _radius;
}

float GraphicBall::jumpingPosX() const
{
    return _jumpingPosX;
}

float GraphicBall::fallingPosX() const
{
    return _fallingPosX;
}


const JBTypes::vec3f &GraphicBall::lookTowardsAsVector() const
{
    return _lookTowardsAsVector;
}


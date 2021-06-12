/*
 * File: BallState.h
 * Author: Morgenthaler S
 *
 * Created on 19 septembre 2020, 07h50
 */
#include "BallState.h"

BallState::BallState(const Ball& ball):
    _ball(ball),
    _coveredRotation(ball.getCoveredRotation()),
    _position(ball.get3DPosition()),
    _burnCoefficient(ball.burnCoefficient()),
    _currentSideAsVector(ball.currentSideAsVector()),
    _stateOfLifeStatus(getStateOfLifeStatus()),
    _radius(ball.getRadius()),
    _crushingCoeff(ball.getCrushingCoefficient()),
    _teleportationCoeff(ball.getTeleportationCoefficient()),
    _teleportationColor(ball.getTeleportationColor()),
    _timeSecondsSinceStateOfLife(ball.getTimeSecondsSinceStateOfLife())
{
}

State::GlobalState BallState::update() {
    _coveredRotation = _ball.getCoveredRotation();
    _position = _ball.get3DPosition();
    _burnCoefficient = _ball.burnCoefficient();
    _currentSideAsVector = _ball.currentSideAsVector();
    _stateOfLifeStatus = getStateOfLifeStatus();
    _radius = _ball.getRadius();
    _crushingCoeff = _ball.getCrushingCoefficient();
    _teleportationCoeff = _ball.getTeleportationCoefficient();
    _teleportationColor = _ball.getTeleportationColor();
    _timeSecondsSinceStateOfLife = _ball.getTimeSecondsSinceStateOfLife();

    const Ball::StateOfLife stateOfLife = _ball.stateOfLife();
    if (stateOfLife == Ball::StateOfLife::Bursting) {
        return State::GlobalState::Separate;
    }
    if (stateOfLife == Ball::StateOfLife::Dead) {
        return State::GlobalState::Dead;
    }
    return State::GlobalState::United;
}

float BallState::teleportationCoeff() const {
    return _teleportationCoeff;
}

const JBTypes::Color& BallState::teleportationColor() const {
    return _teleportationColor;
}

State::DynamicValues <float> BallState::getDynamicFloats() const {
    return {
        { "ballRadius", _radius },
        { "crushingCoeff", _crushingCoeff },
        { "status", _stateOfLifeStatus },
        { "timeStateOfLife", _timeSecondsSinceStateOfLife },
        { "burningCoeff", _burnCoefficient }
    };
}

State::DynamicValues <JBTypes::vec3f> BallState::getDynamicVec3fs() const {
    return {
        { "sideDir", _currentSideAsVector },
        { "position", _position }
    };
}

State::DynamicValues <JBTypes::Quaternion> BallState::getDynamicQuaternions() const {
    return {
        { "quaternion", _coveredRotation }
    };
}

float BallState::getStateOfLifeStatus() const {
    const auto& stateOfLife = _ball.stateOfLife();
    if (
        stateOfLife == Ball::StateOfLife::Normal ||
        stateOfLife == Ball::StateOfLife::Burning ||
        stateOfLife == Ball::StateOfLife::Sliding
        ) {
        return 0.f;
    }
    if (stateOfLife == Ball::StateOfLife::Bursting) {
        return 1.f;
    }
    return 2.f;
}

/*
 * File: BallState.h
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
    _state(ball.state()),
    _stateOfLife(ball.stateOfLife()),
    _radius(ball.getRadius()),
    _lookTowardsAsVector(ball.lookTowardsAsVector()),
    _currentMovementRotation(ball.movementRotation()),
    _crushingCoeff(ball.getCrushingCoefficient()),
    _teleportationCoeff(ball.getTeleportationCoefficient()),
    _teleportationColor(ball.getTeleportationColor()),
    _timeSecondsSinceAction(ball.getTimeSecondsSinceAction()),
    _timeSecondsSinceStateOfLife(ball.getTimeSecondsSinceStateOfLife()),
    _nextLook(JBTypesMethods::directionAsVector(ball.getNextBlockInfo().nextLook)) {
}

void BallState::update() {
    _position = _ball.get3DPosition();
    _burnCoefficient = _ball.burnCoefficient();
    _currentSideAsVector = _ball.currentSideAsVector();
    _state = _ball.state();
    _stateOfLife = _ball.stateOfLife();
    _radius = _ball.getRadius();
    _lookTowardsAsVector = _ball.lookTowardsAsVector();
    _currentMovementRotation = _ball.movementRotation();
    _crushingCoeff = _ball.getCrushingCoefficient();
    _teleportationCoeff = _ball.getTeleportationCoefficient();
    _teleportationColor = _ball.getTeleportationColor();
    _timeSecondsSinceAction = _ball.getTimeSecondsSinceAction();
    _timeSecondsSinceStateOfLife = _ball.getTimeSecondsSinceStateOfLife();
    _nextLook = JBTypesMethods::directionAsVector(_ball.getNextBlockInfo().nextLook);
}

const JBTypes::vec3f& BallState::get3DPosition() const noexcept{
    return _position;
}

float BallState::burnCoefficient() const {
    return _burnCoefficient;
}

const JBTypes::vec3f& BallState::currentSideAsVector() const {
    return _currentSideAsVector;
}

float BallState::getTimeSecondsSinceAction() const {
    return _timeSecondsSinceAction;
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

const JBTypes::vec3f& BallState::lookTowardsAsVector() const {
    return _lookTowardsAsVector;
}

const JBTypes::vec3f& BallState::currentMovementRotation() const {
    return _currentMovementRotation;
}

const std::vector <JBTypes::Dir>& BallState::coveredRotation() const noexcept{
    return _coveredRotation;
}

float BallState::crushingCoeff() const noexcept{
    return _crushingCoeff;
}

float BallState::teleportationCoeff() const {
    return _teleportationCoeff;
}

const ClassicalMechanics& BallState::getMechanicsJumping() const noexcept{
    return _ball.getMechanicsJumping();
}

const JBTypes::Color& BallState::teleportationColor() const {
    return _teleportationColor;
}

const JBTypes::vec3f& BallState::nextLook() const {
    return _nextLook;
}

std::map <std::string, float> BallState::getDynamicFloats() const {
    // TODO: do that in the update function
    const auto getStatus = [this] ()->float {
                               if (
                                   _stateOfLife == Ball::StateOfLife::Normal ||
                                   _stateOfLife == Ball::StateOfLife::Burning ||
                                   _stateOfLife == Ball::StateOfLife::Sliding
                                   ) {
                                   return 0.f;
                               }
                               if (_ball.stateOfLife() == Ball::StateOfLife::Bursting) {
                                   return 1.f;
                               }
                               return 2.f;
                           };
    return {
        { "ballRadius", _radius },
        { "crushingCoeff", _crushingCoeff },
        { "status", getStatus() },
        { "timeStateOfLife", _timeSecondsSinceStateOfLife }
        // TODO: use directly ball method
    };
}

std::map <std::string, JBTypes::vec3f> BallState::getDynamicVec3fs() const {
    return {
        { "sideDir", _currentSideAsVector },
        { "position", _position }
    };
}

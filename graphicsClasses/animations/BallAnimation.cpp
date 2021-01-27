
/* 
 * File:   BallAnimation.cpp
 * Author: Morgenthaler S
 * 
 * Created on 5 janvier 2020, 11:29
 */

#include "BallAnimation.h"
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

BallAnimation::BallAnimation(const BallState &ball):
    _ball(ball) ,
    _computedRotations(0),
    _scale(1.f),
    _rotation(1.f),
    _coveredRotation(1.f),
    _referenceTimePointAction(ball.getTimeAction())
{
}

void BallAnimation::animationAlive() {

    // rotation
    constexpr float sizeBlock = 1.f; // TODO: use a member in Block or map class
    const float angleToGetBlock = sizeBlock/_ball.getRadius();

    for (size_t i = _computedRotations;
         i < _ball.coveredRotation().size();
         ++i)
    {
        const JBTypes::Dir rotationDirection = _ball.coveredRotation().at(i);
        const JBTypes::vec3f rotationVector =
            JBTypesMethods::directionAsVector(rotationDirection);
        const glm::vec3 glmRotationVector
            {rotationVector.x, rotationVector.y, rotationVector.z};
        const glm::quat quaternion(angleToGetBlock* glmRotationVector);
        _coveredRotation = glm::toMat4(quaternion) * _coveredRotation;
    }
    _computedRotations = _ball.coveredRotation().size();

    const JBTypes::vec3f& currentRotationVec = _ball.currentMovementRotation();
    const glm::vec3 currentGlmRotationVector
        {currentRotationVec.x, currentRotationVec.y, currentRotationVec.z};
    const glm::quat quaternion(angleToGetBlock* currentGlmRotationVector);

    // scale & translation
    constexpr float minScaleCrushing = 0.8f;
    const float crushingScale = _ball.crushingCoeff() * minScaleCrushing + 
        (1.f - _ball.crushingCoeff());
    const JBTypes::vec3f sideDir = _ball.currentSideAsVector();
    const glm::vec3 glmSideDir{
        sideDir.x, sideDir.y, sideDir.z};
    const glm::vec3 deformationVector{fabs(sideDir.x),
                                      fabs(sideDir.y),
                                      fabs(sideDir.z)};
    const glm::vec3 scaleVector(
        glm::vec3(1.f) - (1.f-crushingScale) * deformationVector);
    const glm::mat4 scaleMatrix = glm::scale(scaleVector);

    const glm::vec3 translationVector((crushingScale - 1.f) *
                                      _ball.getRadius() * glmSideDir);

    _scale = scaleMatrix;
    _translation = glm::translate(translationVector);
    _rotation = glm::toMat4(quaternion) * _coveredRotation;
}

void BallAnimation::animationBursting() {
    
    constexpr float durationBursting      = 0.07f;
    constexpr float radiusScalarBursting  = 2.5f;
    const float timeSecondsSinceStateOfLife =
        JBTypesMethods::getTimeSecondsSinceTimePoint(
            _ball.getTimeStateOfLife()
        );
    
    const float t = timeSecondsSinceStateOfLife / durationBursting;
    
    const float scaleBursting = (t > 1.f)
        ? 0.f
        : (1.f-t) + radiusScalarBursting * t;
    
    const glm::vec3 scaleVec3   = glm::vec3(scaleBursting);
    const glm::mat4 scaleMatrix = glm::scale(scaleVec3);
    
    _scale                      = scaleMatrix;
}

void BallAnimation::animationDead() {
    _scale = glm::mat4(0.f);
}

void BallAnimation::updateTrans() {
    
    if (_ball.getTimeAction() > _referenceTimePointAction) {
        _referenceTimePointAction = _ball.getTimeAction();
    }
    if (_ball.stateOfLife() == Ball::StateOfLife::Normal || _ball.stateOfLife()
             == Ball::StateOfLife::Burning || _ball.stateOfLife() == 
             Ball::StateOfLife::Sliding) {
        animationAlive();
    } else if (_ball.stateOfLife() == Ball::StateOfLife::Bursting) {
        animationBursting();
    } else if (_ball.stateOfLife() == Ball::StateOfLife::Dead) {
        animationDead();
    }

    //For our animations, we apply the scale after the rotation
    _scaleRotation = _scale * _rotation;
    _model = _translation * _scaleRotation;

}

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   BallAnimation.cpp
 * Author: seb
 * 
 * Created on 5 janvier 2020, 11:29
 */

#include "BallAnimation.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp> 
#include <math.h>

BallAnimation::BallAnimation(const Ball& ball):
    _ball(ball) ,
    _rotationBeforeMovement(1.f),
    _scaleBeforeMovement(1.f),
    _translationBeforeMovement(0.f),
    _referenceTimePoint(ball.getTimeActionMs()),
    _referenceState(ball.state())
{
    
}

void BallAnimation::updateTrans() {
    
    if (_ball.getTimeActionMs() > _referenceTimePoint) {
        _referenceTimePoint = _ball.getTimeActionMs();
        if ( _ball.state() == _referenceState) {
      _scaleBeforeMovement        = glm::vec3(1.f,1.f,1.f);
        _translationBeforeMovement  = glm::vec3(0.f,0.f,0.f);
        }
        else _referenceState = _ball.state();
    }

    if ( _ball.state() == Ball::State::Staying) {

        constexpr float durationAnimation = 0.7f; 
        constexpr float maxCrushing       = 0.2f; 

        const float angleInCosinusFunc    = _ball.getTimeSecondsSinceAction() *
                                        2.f * static_cast<float>(M_PI) /
                                        durationAnimation; 
                                       
        //-cos(0) = -1; 
        const float crushingCoefficient   = (-cosf(angleInCosinusFunc) * 
                                            maxCrushing)/2.f + 
                                            maxCrushing/2.f ;
         
        const JumperBallTypes::vec3f sideDir = _ball.currentSideAsVector();
        const glm::vec3 glmSideDir{
                              sideDir.x, sideDir.y, sideDir.z};
        const glm::vec3 deformationVector { fabs(sideDir.x),
                                            fabs(sideDir.y),
                                            fabs(sideDir.z)};
        const glm::vec3 scaleVector (glm::vec3(1.f) -  crushingCoefficient * 
                                                       deformationVector) ;
        const glm::mat4 scaleMatrix = glm::scale(scaleVector);

        const glm::vec3 translationVector ( -crushingCoefficient *
                                            _ball.getRadius() * glmSideDir);
        
        _scale                      = scaleMatrix;
        _scaleBeforeMovement        = scaleVector;

        _translation                = glm::translate(translationVector);
        _translationBeforeMovement  = translationVector;

        _rotationBeforeMovement     = _rotation; 
    }
    else {
        constexpr float sizeBlock = 1.f;
        float t;
        if (_ball.state() == Ball::State::Moving ||
            _ball.state() == Ball::State::TurningLeft ||
            _ball.state() == Ball::State::TurningRight )
            t = _ball.getTimeSecondsSinceAction()/
                    _ball.timeToGetNextBlock;
        else if (_ball.state() == Ball::State::Jumping) {
            t = _ball.getMechanicsJumping().getPosition(
                    _ball.getTimeSecondsSinceAction()
                    ).x ;
        }
        else  {
            t = _ball.getMechanicsFalling().getPosition(
                    _ball.getTimeSecondsSinceAction()
                    ).x ;
        }
        
        const float angleToGetBlock = sizeBlock/_ball.getRadius();
        const float currentAngle = t * angleToGetBlock;
        
        const JumperBallTypes::vec3f lookDir = _ball.lookTowardsAsVector();
        const JumperBallTypes::vec3f sideDir = _ball.currentSideAsVector();
        
        const glm::vec3 glmLookDir{lookDir.x, lookDir.y, lookDir.z};
        const glm::vec3 glmSideDir{sideDir.x, sideDir.y, sideDir.z};
        
        const glm::mat4 rotationMatrix = glm::rotate( currentAngle,
                glm::cross( glmSideDir, glmLookDir));


        if (_ball.state() == Ball::State::TurningLeft ||
            _ball.state() == Ball::State::TurningRight){
            _rotationBeforeMovement = _rotation; 
        } else {
            _rotation    = rotationMatrix     * _rotationBeforeMovement;
        }
    
        if (t > 1.f ) t = 1.f;
        if (t < 0.f)  t = fabs(t);
        
        _scale        = glm::scale  ( 
         _scaleBeforeMovement + t * ( glm::vec3(1.f) - _scaleBeforeMovement)
                                    );
        _translation  = glm::translate  ( _translationBeforeMovement * (1.f-t));

    }
}


BallAnimation::~BallAnimation() {
}




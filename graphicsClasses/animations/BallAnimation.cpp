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
    _rotationBeforeMovement(1.f)
{
    
}

void BallAnimation::updateTrans() {
    
    // SCALE & TRANSLATE

    if ( _ball.state() == Ball::State::Staying) {

        constexpr float durationAnimation = 0.5f; 
        constexpr float maxCrushing       = 0.2f; 

        const float angleInCosinusFunc    = _ball.getTimeSecondsSinceAction() *
                                        2.f * static_cast<float>(M_PI) /
                                        durationAnimation; 
                                       
        //-cos(0) = -1; 
        const float crushingCoefficient   = (-cosf(angleInCosinusFunc) * 
                                            maxCrushing)/2.f + 
                                            maxCrushing/2.f ;
         
        std::cout << _ball.getTimeSecondsSinceAction() << std::endl;
        std::cout << crushingCoefficient << std::endl;
        const std::array<float,3> sideDir = _ball.currentSideAsVector();
        const glm::vec3 glmSideDir{
                              sideDir.at(0), sideDir.at(1), sideDir.at(2)};
        const glm::vec3 deformationVector { fabs(sideDir.at(0)),
                                            fabs(sideDir.at(1)),
                                            fabs(sideDir.at(2))};
        const glm::vec3 scaleVector (glm::vec3(1.f) -  crushingCoefficient * 
                                                       deformationVector) ;
        const glm::mat4 scaleMatrix = glm::scale(scaleVector);
        

        std::cout << crushingCoefficient << std::endl;
        /*std::cout << scaleVector.x << " " << scaleVector.y << " " 
                  << scaleVector.z << std::endl;*/

        _scale       = scaleMatrix;
        _translation = glm::translate(-crushingCoefficient
                                      *_ball.getRadius()*glmSideDir);

    }

    // ROTATION

    if ( _ball.state() == Ball::State::Moving ||
            _ball.state() == Ball::State::Jumping) {
        
        constexpr float sizeBlock = 1.f;
        float t;
        if (_ball.state() == Ball::State::Moving)
            t = _ball.getTimeSecondsSinceAction()/
                    _ball.timeToGetNextBlock;
        else if (_ball.state() == Ball::State::Jumping)
            t = _ball.getMechanics().getPosition(
                    _ball.getTimeSecondsSinceAction()
                    ).x;
        
        const float angleToGetBlock = sizeBlock/_ball.getRadius();
        const float currentAngle = t * angleToGetBlock;
        
        const std::array<float,3> lookDir = _ball.lookTowardsAsVector();
        const std::array<float,3> sideDir = _ball.currentSideAsVector();
        
        const glm::vec3 glmLookDir{lookDir.at(0), lookDir.at(1), lookDir.at(2)};
        const glm::vec3 glmSideDir{sideDir.at(0), sideDir.at(1), sideDir.at(2)};
        
        const glm::mat4 rotationMatrix = glm::rotate( currentAngle,
                glm::cross( glmSideDir,
                glmLookDir));
        _rotation    = rotationMatrix * _rotationBeforeMovement;
        _scale       = glm::mat4(1.f);
        _translation = glm::mat4(1.f);
        std::cout << "Move" << std::endl;
    }
    else {
        _rotationBeforeMovement = _rotation; 
    }

    /*else if ( _ball.state() == Ball::State::Staying || 
     _ball.state() == Ball::State::TurningLeft ||
     _ball.state() == Ball::State::TurningRight ) {
     }*/
}


BallAnimation::~BallAnimation() {
}




/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   BallAnimation.h
 * Author: Morgenthaler S
 *
 * Created on 5 janvier 2020, 11:29
 */

#ifndef BALLANIMATION_H
#define BALLANIMATION_H
#include "scene/BallState.h"
#include "Animation.h"
#include "../Utility.h"

class BallAnimation : public Animation {
public:

    //--CONSTRUCTORS & DESTRUCTORS--//
    BallAnimation                 (const BallState& ball);
    virtual ~BallAnimation        ()                                  = default;


    //----------METHODS------------//
    void                          updateTrans()                        override;
    void                          animationAlive();
    void                          animationBursting();

private:

    //--------ATTRIBUTES-----------//
    const BallState&              _ball;
    glm::mat4                     _rotationBeforeMovement;
    glm::vec3                     _scaleBeforeMovement;
    glm::vec3                     _translationBeforeMovement;

    glm::mat4                     _scale;
    glm::mat4                     _rotation;

    Ball::State                   _referenceState;
    JBTypes::timePointMs          _referenceTimePointAction;
};

#endif /* BALLANIMATION_H */


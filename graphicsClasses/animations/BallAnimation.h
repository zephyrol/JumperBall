/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   BallAnimation.h
 * Author: seb
 *
 * Created on 5 janvier 2020, 11:29
 */

#ifndef BALLANIMATION_H
#define BALLANIMATION_H
#include <Ball.h>
#include "Animation.h"
#include "../Utility.h"

class BallAnimation : public Animation {
public:

    //--CONSTRUCTORS & DESTRUCTORS--//
    BallAnimation                 (const Ball& ball);
    virtual ~BallAnimation        ();


    //----------METHODS------------//
    void                           updateTrans()                       override;
    void                           animationAlive();
    void                           animationBursting();

private:

    //--------ATTRIBUTES-----------//
    const Ball&                   _ball;
    glm::mat4                     _rotationBeforeMovement;
    glm::vec3                     _scaleBeforeMovement;
    glm::vec3                     _translationBeforeMovement;

    JumperBallTypes::timePointMs  _referenceTimePointAction;
    JumperBallTypes::timePointMs  _referenceTimePointStateOfLife;

    Ball::State                   _referenceState;
    Ball::StateOfLife             _referenceStateOfLife;

};

#endif /* BALLANIMATION_H */


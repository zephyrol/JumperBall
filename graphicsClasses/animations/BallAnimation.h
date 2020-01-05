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
    BallAnimation                 (const Ball& ball);
    virtual ~BallAnimation        ();

    void                           updateTrans()                       override;

private:
    const Ball&                   _ball;
    glm::mat4                     _rotationBeforeMovement;

};

#endif /* BALLANIMATION_H */


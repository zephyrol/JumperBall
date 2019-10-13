/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Ball.h
 * Author: seb
 *
 * Created on 1 octobre 2019, 21:17
 */

#ifndef BALL_H
#define BALL_H
#include "Types.h"

class Ball {
public:
    Ball();
    Ball(const Ball& orig);
    void turnLeft() noexcept;
    void turnRight() noexcept;
    void goStraightOn() noexcept;
    virtual ~Ball();

private:
    unsigned int _currentBlockX;
    unsigned int _currentBlockY;
    unsigned int _currentBlockZ;
    float _3DPosX;
    float _3DPosY;
    float _3DPosZ;

    ballJumperTypes::Direction _currentSide;
    ballJumperTypes::Direction _lookTowards;
        
};

#endif /* BALL_H */


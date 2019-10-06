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
    virtual ~Ball();

private:
    unsigned int _currentBlockX;
    unsigned int _currentBlockY;
    unsigned int _currentBlockZ;
    double _3DPosX;
    double _3DPosY;
    double _3DPosZ;

    ballJumperTypes::Orientation _orientation;
        
};

#endif /* BALL_H */


/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Ball.cpp
 * Author: seb
 * 
 * Created on 1 octobre 2019, 21:17
 */

#include "Ball.h"

Ball::Ball(): _currentBlockX(0),
         _currentBlockY(0),
         _currentBlockZ(0),
         _3DPosX(0.f),
         _3DPosY(0.f),
         _3DPosZ(0.f),
         _orientation(ballJumperTypes::Orientation::Up),
         _direction(ballJumperTypes::Direction::North)
{
}

/*Ball::Ball(const Ball& orig):
{
}*/

Ball::~Ball() {
}


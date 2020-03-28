/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Clock.cpp
 * Author: Morgenthaler S
 * 
 * Created on 27 mars 2020, 20:48
 */

#include "Clock.h"

Clock::Clock(const std::shared_ptr<const Block>& block,
             JumperBallTypes::Direction side) :
Object (block,side) 
{
}

Object::CategoryOfObjects Clock::getCategory() {
    return Object::CategoryOfObjects::Clock;
}

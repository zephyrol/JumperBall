/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Object.cpp
 * Author: seb
 * 
 * Created on 27 mars 2020, 19:40
 */

#include "Object.h"

Object::Object( const std::shared_ptr<const Block>& block, 
                JumperBallTypes::Direction side): 
_block(block),
_side(side),
_gotten(false)
{
}


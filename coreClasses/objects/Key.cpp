/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Key.cpp
 * Author: seb
 * 
 * Created on 27 mars 2020, 20:45
 */

#include "Key.h"

Key::Key(const Block& block, JumperBallTypes::Direction side) :
Object(block, side)
{
}

Object::CategoryOfObjects Key::getCategory() {
    return Object::CategoryOfObjects::Key;
}


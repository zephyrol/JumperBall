/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Special.cpp
 * Author: Morgenthaler S
 * 
 * Created on 7 novembre 2020, 10:42
 */
#include "Special.h"

const JBTypes::Color& Special::getColor() const {
    return _color;
}

const JBTypes::timePointMs &Special::creationTime() const {
    return _creationTime;
}

const JBTypes::Dir &Special::direction() const {
    return _direction;
}

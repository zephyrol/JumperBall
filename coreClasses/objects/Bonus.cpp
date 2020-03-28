/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Bonus.cpp
 * Author: Morgenthaler S
 * 
 * Created on 27 mars 2020, 20:48
 */

#include "Bonus.h"

Bonus::Bonus( const std::shared_ptr<const Block>& block,
              JumperBallTypes::Direction side, 
              unsigned int value): 
Object(block,side),
_value(value)
{
}

Bonus::~Bonus() {

}


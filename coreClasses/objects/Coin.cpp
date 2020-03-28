/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Coin.cpp
 * Author: seb
 * 
 * Created on 27 mars 2020, 20:49
 */

#include "Coin.h"

Coin::Coin(const std::shared_ptr<const Block>& block,
           JumperBallTypes::Direction side):
  Bonus(block, side, 100)
{
}

Object::CategoryOfObjects Coin::getCategory() {
    return Object::CategoryOfObjects::Coin;
}


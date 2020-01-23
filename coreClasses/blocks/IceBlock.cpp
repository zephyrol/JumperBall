/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   IceBlock.cpp
 * Author: seb
 * 
 * Created on 5 octobre 2019, 12:49
 */

#include "IceBlock.h"

IceBlock::IceBlock() {
}


IceBlock::~IceBlock() {
}

Block::categoryOfBlocksInFile IceBlock::getType() const {
    
    return Block::categoryOfBlocksInFile::Ice;
}

void IceBlock::interaction(JumperBallTypes:: Direction ballDir) {
    static_cast<void> (ballDir); //To remove warning... We want to do nothing.
                                //Others classes may override the method.
}


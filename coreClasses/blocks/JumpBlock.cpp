/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   JumpBlock.cpp
 * Author: seb
 * 
 * Created on 11 avril 2020, 19:42
 */

#include "JumpBlock.h"

JumpBlock::JumpBlock(const std::array<bool,6>& facesJumpers):
    _facesJumpers (facesJumpers)
{
}


Block::categoryOfBlocksInFile JumpBlock::getType() const {
    return Block::categoryOfBlocksInFile::Jump;
}


std::array<bool, 6 > JumpBlock::faceInfo() const {
    return _facesJumpers;
}


/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   BaseBlock.cpp
 * Author: Morgenthaler S
 * 
 * Created on 5 octobre 2019, 12:53
 */

#include "BaseBlock.h"

BaseBlock::BaseBlock() {
}


Block::categoryOfBlocksInFile BaseBlock::getType() const {
    return Block::categoryOfBlocksInFile::Base;
}



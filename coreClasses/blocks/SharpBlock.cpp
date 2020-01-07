/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SharpBlock.cpp
 * Author: seb
 * 
 * Created on 5 octobre 2019, 12:51
 */

#include "SharpBlock.h"

SharpBlock::SharpBlock() {
}

/*SharpBlock::SharpBlock(const SharpBlock& orig) {
}*/

SharpBlock::~SharpBlock() {
}

Block::categoryOfBlocksInFile SharpBlock::getType() const {
    return Block::categoryOfBlocksInFile::Sharp;
}

void SharpBlock::interaction() {

}

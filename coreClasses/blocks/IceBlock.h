/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   IceBlock.h
 * Author: Morgenthaler S
 *
 * Created on 5 octobre 2019, 12:49
 */

#ifndef ICEBLOCK_H
#define ICEBLOCK_H
#include "Block.h"

class IceBlock : public Block {
public:
    IceBlock();
    IceBlock(const IceBlock& orig);

    Block::categoryOfBlocksInFile getType() const override;

private:

};

#endif /* ICEBLOCK_H */


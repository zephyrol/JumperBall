/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   BaseBlock.h
 * Author: seb
 *
 * Created on 5 octobre 2019, 12:53
 */

#ifndef BASEBLOCK_H
#define BASEBLOCK_H
#include "Block.h"


class BaseBlock : public Block {
public:
    BaseBlock();
    BaseBlock(const BaseBlock& orig);
    virtual ~BaseBlock();

    Block::categoryOfBlocksInFile getType() const override;

private:

};

#endif /* BASEBLOCK_H */


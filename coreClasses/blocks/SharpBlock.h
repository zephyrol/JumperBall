/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SharpBlock.h
 * Author: seb
 *
 * Created on 5 octobre 2019, 12:51
 */

#ifndef SPICYBLOCK_H
#define SPICYBLOCK_H
#include "Block.h"

class SharpBlock : public Block {
public:
    SharpBlock();
    SharpBlock(const SharpBlock& orig);
    virtual ~SharpBlock();

    Block::categoryOfBlocksInFile getType() const override;
    void interaction() override;

private:

};

#endif /* SPICYBLOCK_H */


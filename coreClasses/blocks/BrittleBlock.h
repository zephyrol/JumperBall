/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   BrittleBlock.h
 * Author: seb
 *
 * Created on 5 octobre 2019, 12:50
 */

#ifndef BRITTLEBLOCK_H
#define BRITTLEBLOCK_H
#include "Block.h"

class BrittleBlock : public Block {
public:
    BrittleBlock();
    BrittleBlock(const BrittleBlock& orig);
    virtual ~BrittleBlock();

    Block::categoryOfBlocksInFile getType() const override;
    void interaction() override;
private:

};

#endif /* BRITTLEBLOCK_H */


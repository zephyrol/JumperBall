/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   JumpBlock.h
 * Author: seb
 *
 * Created on 11 avril 2020, 19:42
 */

#ifndef JUMPBLOCK_H
#define JUMPBLOCK_H
#include "Block.h"

class JumpBlock : public Block
{
public:
    //--CONSTRUCTORS & DESTRUCTORS--//
    JumpBlock                             (const std::array<bool,6>& 
                                            facesJumpers);

    //-------CONST METHODS----------//
    Block::categoryOfBlocksInFile         getType()              const override;
    std::array<bool,6>                    faceInfo()             const override;

    
private:
    const std::array<bool,6>              _facesJumpers;

};

#endif /* JUMPBLOCK_H */


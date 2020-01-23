/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Block.h
 * Author: seb
 *
 * Created on 1 octobre 2019, 21:18
 */

#ifndef BLOCK_H
#define BLOCK_H
#include "Types.h"

class Block {
public:

    //--CONSTRUCTORS & DESTRUCTORS--//
    Block                             ();
    virtual                           ~Block() ;


    //------------TYPES------------//
    enum class categoryOfBlocksInFile {None,Base,Fire,Ice,Sharp,Brittle};


    //----------METHODS-------------//
    virtual void                      interaction(JumperBallTypes::Direction 
                                                    ballDir)                 =0;


    //-------CONST METHODS----------//
    virtual                           categoryOfBlocksInFile getType() const =0;
    virtual std::array<bool,6>        faceInfo()                          const;
    virtual bool                      stillExists()                       const;
};

    //--------ATTRIBUTES-----------//

#endif /* BLOCK_H */


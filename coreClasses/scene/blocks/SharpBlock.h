/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SharpBlock.h
 * Author: Morgenthaler S
 *
 * Created on 5 octobre 2019, 12:51
 */

#ifndef SPICYBLOCK_H
#define SPICYBLOCK_H
#include "Block.h"

class SharpBlock : public Block {
public:
    //--CONSTRUCTORS & DESTRUCTORS--//
    SharpBlock                    (const std::array<bool,6>&
                                                      facesSharps);

    //-------CONST METHODS----------//
    std::array<bool,6>            faceInfo()     const override;

    //----------METHODS-------------//
    virtual Block::Effect         interaction( const JBTypes::Dir&  ballDir,
                                               const JBTypes::timePointMs&
                                                currentTime,
                                               const JBTypes::vec3f& posBall,
                                    const std::array<unsigned int,3>& posBlock)
                                                                       override;
     
    
private:
    const std::array<bool,6>      _facesSharps;
};

#endif /* SPICYBLOCK_H */

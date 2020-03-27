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
    Block                             (const std::array<float,9>& 
                                                                localTransform);
    virtual                           ~Block()                         =default;


    //------------TYPES------------//
    enum class categoryOfBlocksInFile {None,Base,Fire,Ice,Sharp,Brittle};


    //----------METHODS-------------//
    virtual void                      interaction(
                                    const JumperBallTypes::Direction& 
                                            ballDir,
                                    const JumperBallTypes::timePointMs&
                                            currentTime,
                                    const JumperBallTypes::vec3f& posBall,
                                    const std::array<unsigned int,3>& posBlock);
    virtual void                      detectionEvent(
                                        const JumperBallTypes::Direction&
                                                ballDir,
                                        const JumperBallTypes::timePointMs&
                                                currentTime
                                                );


    //-------CONST METHODS----------//
    virtual                           categoryOfBlocksInFile getType() const =0;
    virtual std::array<bool,6>        faceInfo()                          const;
    virtual bool                      stillExists()                       const;
    virtual bool                      burstBall()                         const;
    virtual const std::array<float,9>&       
                                      localTransform()                    const;

    //--------ATTRIBUTES-----------//
    std::array<float,9>               _localTransform;


};


#endif /* BLOCK_H */


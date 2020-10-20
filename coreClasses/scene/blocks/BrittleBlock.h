/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   BrittleBlock.h
 * Author: Morgenthaler S
 *
 * Created on 5 octobre 2019, 12:50
 */

#ifndef BRITTLEBLOCK_H
#define BRITTLEBLOCK_H
#include "Block.h"

class BrittleBlock : public Block {
public:
    BrittleBlock                  ();

    bool                          stillExists()                  const override;

    void                          setFallDirection (JBTypes:: Direction 
                                                             ballDir);

    virtual Block::Effect         interaction(
                                    const JBTypes::Dir& 
                                            ballDir,
                                    const JBTypes::timePointMs&
                                            currentTime,
                                    const JBTypes::vec3f& posBall,
                                    const std::array<unsigned int,3>& posBlock)
                                                                       override;

    virtual Block::Effect         detectionEvent(
                                    const JBTypes::Dir& 
                                            ballDir,
                                    const JBTypes::timePointMs&
                                            currentTime)               override;

private:
    bool                          _stillThere;
    bool                          _isGoingToBreak;
    JBTypes::timePointMs          _collisionTime;
    JBTypes::timePointMs          _timeUpdate;
    JBTypes::Dir                  _fallDirection;
};

#endif /* BRITTLEBLOCK_H */

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

    Block::categoryOfBlocksInFile getType()                      const override;

    bool                          stillExists()                  const override;

    void                          setFallDirection (JumperBallTypes:: Direction 
                                                             ballDir);

    virtual void                  interaction(
                                    const JumperBallTypes::Direction& 
                                            ballDir,
                                    const JumperBallTypes::timePointMs&
                                            currentTime,
                                    const JumperBallTypes::vec3f& posBall,
                                    const std::array<unsigned int,3>& posBlock)
                                                                       override;

    virtual void                  detectionEvent(
                                    const JumperBallTypes::Direction& 
                                            ballDir,
                                    const JumperBallTypes::timePointMs&
                                            currentTime)               override;
    
    virtual const std::array<float,9>&       
                                  localTransform()               const override;

private:
    bool                          _stillThere;
    bool                          _isGoingToBreak;
    JumperBallTypes::timePointMs  _collisionTime;
    JumperBallTypes::timePointMs  _timeUpdate;
    JumperBallTypes::Direction    _fallDirection;
};

#endif /* BRITTLEBLOCK_H */


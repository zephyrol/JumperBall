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
    BrittleBlock                  ();
    virtual ~BrittleBlock         ();

    Block::categoryOfBlocksInFile getType()                      const override;
    void                          interaction(JumperBallTypes::Direction 
                                                ballDir)               override;
    bool                          stillExists()                  const override;

    void                          setFallDirection (JumperBallTypes:: Direction 
                                                             ballDir);

    

private:
    bool                          _stillThere;
    bool                          _isGoingToBreak;
    std::chrono::time_point<std::chrono::system_clock> 
                                  _collisionTime;
    JumperBallTypes::Direction    _fallDirection;
};

#endif /* BRITTLEBLOCK_H */


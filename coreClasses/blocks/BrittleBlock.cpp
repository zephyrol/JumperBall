/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   BrittleBlock.cpp
 * Author: seb
 * 
 * Created on 5 octobre 2019, 12:50
 */

#include "BrittleBlock.h"

BrittleBlock::BrittleBlock():
  _stillThere(true),
  _isGoingToBreak(false),
  _collisionTime(),
  _fallDirection(JumperBallTypes::Direction::Down)
{
}


Block::categoryOfBlocksInFile BrittleBlock::getType() const {

    return Block::categoryOfBlocksInFile::Brittle;
}

void BrittleBlock::interaction(JumperBallTypes:: Direction ballDir) {
    if (!_isGoingToBreak) {
        _collisionTime = std::chrono::system_clock::now();
        _isGoingToBreak = true;
        setFallDirection(ballDir);
    }
}

void BrittleBlock::setFallDirection(JumperBallTypes::Direction ballDir) {
    switch (ballDir) {
        case JumperBallTypes::Direction::South :
            _fallDirection = JumperBallTypes::Direction::North;
            break;
        case JumperBallTypes::Direction::North :
            _fallDirection = JumperBallTypes::Direction::South;
            break;
        case JumperBallTypes::Direction::East :
            _fallDirection = JumperBallTypes::Direction::West;
            break;
        case JumperBallTypes::Direction::West :
            _fallDirection = JumperBallTypes::Direction::East;
            break;
        case JumperBallTypes::Direction::Up :
            _fallDirection = JumperBallTypes::Direction::Down;
            break;
        case JumperBallTypes::Direction::Down :
            _fallDirection = JumperBallTypes::Direction::Up;
            break;
        default :
            break;
    }
      
}



bool BrittleBlock::stillExists() const {
    return _stillThere;
}



BrittleBlock::~BrittleBlock() {
}
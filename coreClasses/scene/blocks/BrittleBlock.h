/*
 * File: BrittleBlock.h
 * Author: Morgenthaler S
 *
 * Created on 5 octobre 2019, 12:50
 */

#ifndef BRITTLEBLOCK_H
#define BRITTLEBLOCK_H
#include "Block.h"

class BrittleBlock:public Block {
public:
BrittleBlock(const JBTypes::vec3ui& position);

bool stillExists() const override;

void setFallDirection(JBTypes::Direction
                      ballDir);

virtual Block::Effect interaction(
    const JBTypes::Dir&
    ballDir,
    const JBTypes::timePointMs&
    currentTime,
    const JBTypes::vec3f& posBall,
    const JBTypes::vec3ui& posBlock)
override;

virtual Block::Effect detectionEvent(
    const JBTypes::Dir&
    ballDir,
    const JBTypes::timePointMs&
    currentTime) override;

private:
bool _stillThere;
bool _isGoingToBreak;
JBTypes::timePointMs _collisionTime;
JBTypes::timePointMs _timeUpdate;
JBTypes::Dir _fallDirection;
};

#endif /* BRITTLEBLOCK_H */

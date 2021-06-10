/*
 * File: GhostBlock.h
 * Author: Morgenthaler S
 *
 * Created on 20 aout 2020, 21:33
 */

#ifndef GHOSTBLOCK_H
#define GHOSTBLOCK_H
#include "Block.h"

class GhostBlock:public Block {
public:
GhostBlock(const JBTypes::vec3ui& position, float periodicity = 3.f);
bool isExists() const override;
virtual Block::Effect interaction(
    const JBTypes::Dir& ballDir,
    const JBTypes::timePointMs& currentTime,
    const JBTypes::vec3f& posBall
    )
override;

private:
const float _periodicity;
const JBTypes::timePointMs _creationTime;
bool _isThere;
};

#endif /* GHOSTBLOCK_H */

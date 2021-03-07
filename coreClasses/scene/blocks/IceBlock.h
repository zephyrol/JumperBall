/*
 * File: IceBlock.h
 * Author: Morgenthaler S
 *
 * Created on 5 octobre 2019, 12:49
 */

#ifndef ICEBLOCK_H
#define ICEBLOCK_H
#include "Block.h"

class IceBlock:public Block {
public:
IceBlock(const JBTypes::vec3ui& position);

virtual Effect detectionEvent(
    const JBTypes::Dir& ballDir,
    const JBTypes::timePointMs& currentTime
    );
};

#endif /* ICEBLOCK_H */

/*
 * File: FireBlock.h
 * Author: Morgenthaler S
 *
 * Created on 5 octobre 2019, 12:50
 */

#ifndef FIREBLOCK_H
#define FIREBLOCK_H
#include "Block.h"

class FireBlock:public Block {
public:
FireBlock(const JBTypes::vec3ui& position);

virtual Effect detectionEvent(
    const JBTypes::Dir& ballDir,
    const JBTypes::timePointMs& currentTime
    );
};

#endif /* FIREBLOCK_H */

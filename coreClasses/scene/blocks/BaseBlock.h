/*
 * File: BaseBlock.h
 * Author: Morgenthaler S
 *
 * Created on 5 octobre 2019, 12:53
 */

#ifndef BASEBLOCK_H
#define BASEBLOCK_H
#include "Block.h"


class BaseBlock:public Block {
public:
BaseBlock(const JBTypes::vec3ui& position);
};

#endif /* BASEBLOCK_H */

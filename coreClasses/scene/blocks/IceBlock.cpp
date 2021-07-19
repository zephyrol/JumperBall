/*
 * File: IceBlock.cpp
 * Author: Morgenthaler S
 *
 * Created on 5 octobre 2019, 12:49
 */

#include "IceBlock.h"

IceBlock::IceBlock(const JBTypes::vec3ui& position):Block(position) {
}

unsigned char IceBlock::getBlockSymbol() const {
    return 'I';
}

Block::Effect IceBlock::detectionEvent (const JBTypes::Dir&,
                                        const JBTypes::timePointMs&) {
    return Block::Effect::Slide;
}

/*
 * File: JumpBlock.cpp
 * Author: seb
 *
 * Created on 11 avril 2020, 19:42
 */

#include "JumpBlock.h"

JumpBlock::JumpBlock(const JBTypes::vec3ui &position,
                     const vecItem_sptr &items,
                     const vecEnemy_sptr &enemies,
                     const vecSpecial_sptr &specials,
                     const Ball_sptr& ball,
                     const std::array<bool, 6> &facesJumpers):
                     Block(position, ball, items, enemies, specials),
                     _facesJumpers(facesJumpers) {
}

std::array <bool, 6> JumpBlock::faceInfo() const {
    return _facesJumpers;
}

Block::Effect JumpBlock::detectionEvent (const JBTypes::Dir& direction,
                                         const JBTypes::timePointMs&) {
    const unsigned int dir = JBTypesMethods::directionAsInteger(direction);
    return faceInfo().at(dir) ? Block::Effect::Jump : Block::Effect::Nothing;
}


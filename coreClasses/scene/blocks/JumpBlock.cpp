/*
 * File: JumpBlock.cpp
 * Author: seb
 *
 * Created on 11 avril 2020, 19:42
 */

#include "JumpBlock.h"

JumpBlock::JumpBlock(const std::array <bool, 6>& facesJumpers):
    _facesJumpers(facesJumpers) {
}

std::array <bool, 6> JumpBlock::faceInfo() const {
    return _facesJumpers;
}

Block::Effect JumpBlock::detectionEvent (const JBTypes::Dir& ballDir,
                                         const JBTypes::timePointMs&) {
    Block::Effect effect;
    const unsigned int dir = JBTypesMethods::directionAsInteger(ballDir);
    if (faceInfo().at(dir)) {
        effect = Block::Effect::Jump;
    } else {
        effect = Block::Effect::Nothing;
    }
    return effect;
}

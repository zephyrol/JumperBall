/*
 * File: JumpBlock.cpp
 * Author: seb
 *
 * Created on 11 avril 2020, 19:42
 */

#include "JumpBlock.h"

JumpBlock::JumpBlock(const JBTypes::vec3ui& position, const std::array <bool, 6>& facesJumpers):
    Block(position),
    _facesJumpers(facesJumpers) {
}

std::array <bool, 6> JumpBlock::faceInfo() const {
    return _facesJumpers;
}

unsigned char JumpBlock::getBlockSymbol() const {
    return 'J';
}

std::string JumpBlock::getBlockOptions() const {
    std::string options;
    const std::array<unsigned char, 6> directions { 'N', 'S', 'E', 'W', 'U', 'D'};
    for (size_t i = 0; i < _facesJumpers.size(); ++i) {
        if (_facesJumpers.at(i)) {
            options.push_back(directions.at(i));
        }
    } 
    
    return options;
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

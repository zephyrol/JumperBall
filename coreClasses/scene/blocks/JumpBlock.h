/*
 * File: JumpBlock.h
 * Author: seb
 *
 * Created on 11 avril 2020, 19:42
 */

#ifndef JUMPBLOCK_H
#define JUMPBLOCK_H
#include "Block.h"

class JumpBlock:public Block {
public:
// --CONSTRUCTORS & DESTRUCTORS--//
JumpBlock(const std::array <bool, 6>&
          facesJumpers);

// -------CONST METHODS----------//
std::array <bool, 6> faceInfo() const override;


// ----------METHODS-------------//
virtual Effect detectionEvent(
    const JBTypes::Dir& ballDir,
    const JBTypes::timePointMs& currentTime
    ) override;

private:
const std::array <bool, 6> _facesJumpers;

};

#endif /* JUMPBLOCK_H */

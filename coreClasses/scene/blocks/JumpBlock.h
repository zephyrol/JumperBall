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
    JumpBlock(
        const JBTypes::vec3ui& position,
        const vecItem_sptr& items,
        const vecEnemy_sptr& enemies,
        const vecSpecial_sptr& specials,
        const Ball_sptr& ball,
        const std::array <bool, 6>& facesJumpers
    );

    std::array <bool, 6> faceInfo() const override;

Effect detectionEvent(
    const JBTypes::Dir& direction,
    const JBTypes::timePointMs& currentTime
) override;

vecCstShape_sptr getExtraShapes() const override;


private:
const std::array <bool, 6> _facesJumpers;

};

#endif /* JUMPBLOCK_H */

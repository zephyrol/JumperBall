/*
 * File: JumpBlock.h
 * Author: S.Morgenthaler
 *
 * Created on 11 avril 2020, 19:42
 */

#ifndef JUMPBLOCK_H
#define JUMPBLOCK_H

#include "InteractiveBlock.h"

class JumpBlock : public InteractiveBlock {
public:
    JumpBlock(
        const JBTypes::vec3ui &position,
        const vecItem_sptr &items,
        const vecEnemy_sptr &enemies,
        const vecSpecial_sptr &specials,
        const Ball_sptr &ball,
        const std::array<bool, 6> &facesJumpers
    );

    std::array<bool, 6> faceInfo() const override;

    Effect detectionEvent() override;

    vecCstShape_sptr getExtraShapes() const override;


private:
    const std::array<bool, 6> _facesJumpers;

};

#endif /* JUMPBLOCK_H */

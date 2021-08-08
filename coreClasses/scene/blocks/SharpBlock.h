/*
 * File: SharpBlock.h
 * Author: Morgenthaler S
 *
 * Created on 5 octobre 2019, 12:51
 */

#ifndef SPICYBLOCK_H
#define SPICYBLOCK_H
#include "Block.h"

class SharpBlock:public Block {
public:

    SharpBlock(
        const JBTypes::vec3ui& position,
        const vecItem_sptr& items,
        const vecEnemy_sptr& enemies,
        const vecSpecial_sptr& specials,
        const Ball_sptr& ball,
        const std::array <bool, 6>& facesSharps
    );

    std::array <bool, 6> faceInfo() const override;

virtual Block::Effect interaction(
    const JBTypes::timePointMs& currentTime
) override;

private:
const std::array <bool, 6> _facesSharps;
};

#endif /* SHARPBLOCK_H */

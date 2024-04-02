/*
 * File: BaseBlock.h
 * Author: Morgenthaler S
 *
 * Created on 5 octobre 2019, 12:53
 */

#ifndef BASEBLOCK_H
#define BASEBLOCK_H

#include "InteractiveBlock.h"


class BaseBlock : public InteractiveBlock {
public:
    BaseBlock(
        const glm::u32vec3 &position,
        const vecItem_sptr &items,
        const vecEnemy_sptr &enemies,
        const vecSpecial_sptr &specials,
        const Ball_sptr &ball
    );
};

#endif /* BASEBLOCK_H */

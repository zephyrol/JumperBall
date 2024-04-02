/*
 * File: BaseBlock.cpp
 * Author: Morgenthaler S
 *
 * Created on 5 octobre 2019, 12:53
 */

#include "BaseBlock.h"

BaseBlock::BaseBlock(const glm::u32vec3 &position,
                     const vecItem_sptr &items,
                     const vecEnemy_sptr &enemies,
                     const vecSpecial_sptr &specials,
                     const Ball_sptr &ball
) :
    InteractiveBlock(position, items, enemies, specials, ball) {
}

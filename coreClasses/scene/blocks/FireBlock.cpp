/*
 * File: FireBlock.cpp
 * Author: Morgenthaler S
 *
 * Created on 5 octobre 2019, 12:50
 */

#include "FireBlock.h"

FireBlock::FireBlock(const JBTypes::vec3ui &position,
                     const vecItem_sptr &items,
                     const vecEnemy_sptr &enemies,
                     const vecSpecial_sptr &specials,
                     const Ball_sptr& ball
                     ):
                     Block(position, ball, items, enemies, specials){
}

Block::Effect FireBlock::detectionEvent (const JBTypes::Dir&, const JBTypes::timePointMs&) {
    return Block::Effect::Burn;
}

JBTypes::Color FireBlock::getColor() const {
    return JBTypes::Color::Orange;
}

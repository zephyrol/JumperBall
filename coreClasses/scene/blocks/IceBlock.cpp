/*
 * File: IceBlock.cpp
 * Author: Morgenthaler S
 *
 * Created on 5 octobre 2019, 12:49
 */

#include "IceBlock.h"

IceBlock::IceBlock(const JBTypes::vec3ui &position,
                   const vecItem_sptr &items,
                   const vecEnemy_sptr &enemies,
                   const vecSpecial_sptr &specials,
                   const Ball_sptr& ball
                   ):
    InteractiveBlock(position, items, enemies, specials, ball){
}

Block::Effect IceBlock::detectionEvent () {
    return Block::Effect::Slide;
}

JBTypes::Color IceBlock::getColor() const {
    return JBTypes::Color::Blue;
}


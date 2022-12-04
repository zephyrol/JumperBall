/*
 * File: FireBlock.cpp
 * Author: Morgenthaler S
 *
 * Created on 5 octobre 2019, 12:50
 */

#include "FireBlock.h"
#include "system/SoundOutput.h"

FireBlock::FireBlock(
    const JBTypes::vec3ui &position,
    const vecItem_sptr &items,
    const vecEnemy_sptr &enemies,
    const vecSpecial_sptr &specials,
    const Ball_sptr &ball
) :
    InteractiveBlock(position, items, enemies, specials, ball) {
}

Block::Effect FireBlock::detectionEvent() {
    _ball.lock()->addUpdateOutput(std::make_shared<SoundOutput>("blockTouchedFire"));
    return Block::Effect::Burn;
}

JBTypes::Color FireBlock::getColor() const {
    return JBTypes::Color::Orange;
}

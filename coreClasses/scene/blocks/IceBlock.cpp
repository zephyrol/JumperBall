/*
 * File: IceBlock.cpp
 * Author: Morgenthaler S
 *
 * Created on 5 octobre 2019, 12:49
 */

#include "IceBlock.h"
#include "system/SoundOutput.h"

IceBlock::IceBlock(
    const glm::u32vec3 &position,
    const vecItem_sptr &items,
    const vecEnemy_sptr &enemies,
    const vecSpecial_sptr &specials,
    const Ball_sptr &ball
) :
    InteractiveBlock(position, items, enemies, specials, ball) {
}

Block::Effect IceBlock::detectionEvent() {
    InteractiveBlock::detectionEvent();
    _ball.lock()->addUpdateOutput(std::make_shared<SoundOutput>("blockTouchedIce"));
    return Block::Effect::Slide;
}

JBTypes::Color IceBlock::getColor() const {
    return JBTypes::Color::Blue;
}


//
// Created by S. on 24/08/2021.
//

#include "InteractiveBlock.h"

InteractiveBlock::InteractiveBlock(
    const JBTypes::vec3ui &position,
    const vecItem_sptr &items,
    const vecEnemy_sptr &enemies,
    const vecSpecial_sptr &specials,
    const Ball_sptr &ball,
    bool alwaysHasInteractions
) :
    Block(position, items, enemies, specials, alwaysHasInteractions),
    _ball(ball) {
}

void InteractiveBlock::catchItem() {
    for (const Item_sptr &item: _items) {
        item->catchingTest();
    }
}

Block::Effect InteractiveBlock::interaction() const {
    for (const auto &enemy: _cstEnemies) {
        const bool isTouching = enemy->touchingTest();
        if (isTouching) {
            return Block::Effect::Burst;
        }
    }
    return Block::Effect::Nothing;
}

void InteractiveBlock::update() {
    Block::update();
    catchItem();
}

Block::Effect InteractiveBlock::detectionEvent() {
    for (const auto &special: _specials) {
        if (special->direction() == _ball.lock()->currentSide()) {
            special->applySpecialEffect();
        }
    }
    return Block::Effect::Nothing;
}

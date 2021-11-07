//
// Created by sebastien on 24/08/2021.
//

#include "InteractiveBlock.h"

InteractiveBlock::InteractiveBlock(
    const JBTypes::vec3ui &position,
    const vecItem_sptr &items,
    const vecEnemy_sptr &enemies,
    const vecSpecial_sptr &specials,
    const Ball_sptr &ball,
    bool alwaysHasInteractions,
    bool isFixed
):
    Block(position,items,enemies,specials,alwaysHasInteractions, isFixed),
    _ball(ball)
{
}

void InteractiveBlock::catchItem() {
    for (const Item_sptr& item : _items) {
        if (!item->isGotten()) {
            const auto ball = _ball.lock();
            item->catchingTest(ball->get3DPosition(), ball->getRadius());
        }
    }
}

Block::Effect InteractiveBlock::interaction() const {
    for (const auto& enemy : _cstEnemies) {
        const bool isTouching = enemy->touchingTest();
        if (isTouching) {
            return Block::Effect::Burst;
        }
    }
    return Block::Effect::Nothing;
}

void InteractiveBlock::update(const JBTypes::timePointMs &updatingTime){
    Block::update(updatingTime);
    catchItem();
}

Block::Effect InteractiveBlock::detectionEvent() {
    for (const auto& special: _specials) {
        special->applySpecialEffect();
    }
    return Block::Effect::Nothing;
}

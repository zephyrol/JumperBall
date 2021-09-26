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

void InteractiveBlock::update(const JBTypes::timePointMs &currentTime) {
    interaction(currentTime);
    for(const auto& enemy : _enemies) {
        enemy->update();
    }
}

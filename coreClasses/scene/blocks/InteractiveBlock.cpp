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

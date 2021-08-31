//
// Created by sebastien on 24/08/2021.
//

#ifndef JUMPERBALLAPPLICATION_INTERACTIVEBLOCK_H
#define JUMPERBALLAPPLICATION_INTERACTIVEBLOCK_H


#include <scene/Ball.h>
#include "Block.h"

class InteractiveBlock : public Block {
public:
    InteractiveBlock(
        const JBTypes::vec3ui &position,
        const vecItem_sptr &items,
        const vecEnemy_sptr &enemies,
        const vecSpecial_sptr &specials,
        const Ball_sptr& ball,
        bool alwaysHasInteractions = false,
        bool isFixed = true
    );

    void catchItem() override;

protected:
    const Ball_sptr _ball;


};


#endif //JUMPERBALLAPPLICATION_INTERACTIVEBLOCK_H

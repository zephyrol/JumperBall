//
// Created by S.Morgenthaler on 24/08/2021.
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
        const Ball_sptr &ball,
        bool alwaysHasInteractions = false
    );

    void update() override;

    Block::Effect detectionEvent() override;

    Block::Effect interaction() const override;

protected:
    void catchItem();

    const Ball_wptr _ball;

};


#endif //JUMPERBALLAPPLICATION_INTERACTIVEBLOCK_H

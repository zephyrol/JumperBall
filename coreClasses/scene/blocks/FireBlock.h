/*
 * File: FireBlock.h
 * Author: Morgenthaler S
 *
 * Created on 5 octobre 2019, 12:50
 */

#ifndef FIREBLOCK_H
#define FIREBLOCK_H
#include "Block.h"

class FireBlock:public Block {
public:

    FireBlock(
        const JBTypes::vec3ui& position,
        const vecItem_sptr& items,
        const vecEnemy_sptr& enemies,
        const vecSpecial_sptr& specials,
        const Ball_sptr& ball
    );

virtual Effect detectionEvent(
    const JBTypes::Dir& direction,
    const JBTypes::timePointMs& currentTime
) override;

    JBTypes::Color getColor() const override;

};

#endif /* FIREBLOCK_H */

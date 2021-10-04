/*
 * File: BrittleBlock.h
 * Author: Morgenthaler S
 *
 * Created on 5 octobre 2019, 12:50
 */

#ifndef BRITTLEBLOCK_H
#define BRITTLEBLOCK_H
#include "InteractiveBlock.h"

class BrittleBlock:public InteractiveBlock {
public:
    BrittleBlock(
        const JBTypes::vec3ui& position,
        const vecItem_sptr& items,
        const vecEnemy_sptr& enemies,
        const vecSpecial_sptr& specials,
        const Ball_sptr& ball
    );

    bool isExists() const override;

void setFallDirection(JBTypes::Direction direction);

void update(const JBTypes::timePointMs &updatingTime) override;

Block::Effect detectionEvent() override;

bool mayDisappear() const override;

JBTypes::Color getColor() const override;
private:
bool _stillThere;
bool _isGoingToBreak;
JBTypes::timePointMs _collisionTime;
JBTypes::Dir _fallDirection;
};

#endif /* BRITTLEBLOCK_H */

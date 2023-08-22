/*
 * File: BrittleBlock.h
 * Author: Morgenthaler S
 *
 * Created on 5 octobre 2019, 12:50
 */

#ifndef BRITTLEBLOCK_H
#define BRITTLEBLOCK_H

#include "InteractiveBlock.h"

class BrittleBlock : public InteractiveBlock {
public:
    BrittleBlock(
        const JBTypes::vec3ui &position,
        const vecItem_sptr &items,
        const vecEnemy_sptr &enemies,
        const vecSpecial_sptr &specials,
        const Ball_sptr &ball
    );

    bool isExists() const override;

    void setFallDirection(JBTypes::Direction direction);

    void update() override;

    DynamicValues <JBTypes::vec3f> getDynamicVec3fValues() const override;

    DynamicValues <JBTypes::Quaternion> getDynamicQuaternionValues() const override;

    Block::Effect detectionEvent() override;

    bool mayDisappear() const override;

    JBTypes::Color getColor() const override;

    std::string getDynamicGroupHash() const override;

private:

    const CstChronometer_sptr _chronometer;
    const JBTypes::vec3f _fPosition;
    bool _stillThere;
    bool _isGoingToBreak;
    float _collisionTime;
    JBTypes::vec3f _fallDirection;
    JBTypes::Quaternion _shakingRotation;
    JBTypes::vec3f _localTranslation;
};

#endif /* BRITTLEBLOCK_H */

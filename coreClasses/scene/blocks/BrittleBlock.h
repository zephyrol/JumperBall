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
        const glm::u32vec3 &position,
        const vecItem_sptr &items,
        const vecEnemy_sptr &enemies,
        const vecSpecial_sptr &specials,
        const Ball_sptr &ball
    );

    bool isExists() const override;

    void setFallingDirection(JBTypes::Dir direction);

    void update() override;

    DynamicValues <glm::vec3> getDynamicVec3fValues() const override;

    DynamicValues <glm::quat> getDynamicQuaternionValues() const override;

    Block::Effect detectionEvent() override;

    bool mayDisappear() const override;

    JBTypes::Color getColor() const override;

    std::string getDynamicGroupHash() const override;

private:

    const CstChronometer_sptr _chronometer;
    const glm::vec3 _fPosition;
    bool _stillThere;
    bool _isGoingToBreak;
    float _collisionTime;
    glm::vec3 _fallingDirection;
    glm::vec3 _fallingRotationAxis;
    glm::quat _shakingRotation;
    glm::vec3 _localTranslation;
};

#endif /* BRITTLEBLOCK_H */

/*
 * File: GhostBlock.h
 * Author: Morgenthaler S
 *
 * Created on 20 aout 2020, 21:33
 */

#ifndef GHOSTBLOCK_H
#define GHOSTBLOCK_H

#include "InteractiveBlock.h"

class GhostBlock : public InteractiveBlock {
public:
    GhostBlock(
        const glm::u32vec3 &position,
        const vecItem_sptr &items,
        const vecEnemy_sptr &enemies,
        const vecSpecial_sptr &specials,
        const Ball_sptr &ball,
        float periodicity = 3.f
    );

    bool isExists() const override;

    void update() override;

    bool mayDisappear() const override;

    JBTypes::Color getColor() const override;

    std::string getDynamicGroupHash() const override;

    DynamicValues <glm::vec3> getDynamicVec3fValues() const override;

private:
    const CstChronometer_sptr _chronometer;
    const float _periodicity;
    const glm::vec3 _fPosition;
    bool _isThere;
    float _scale;
};

#endif /* GHOSTBLOCK_H */

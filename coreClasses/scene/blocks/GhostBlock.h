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
        const JBTypes::vec3ui &position,
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

    Displayable::StaticValues<JBTypes::vec3f> getStaticVec3fValues() const override;

    std::string getDynamicGroupHash() const override;

    bool dynamicsMayChange() const override;


private:
    const CstChronometer_sptr _chronometer;
    const float _periodicity;
    bool _isThere;
};

#endif /* GHOSTBLOCK_H */

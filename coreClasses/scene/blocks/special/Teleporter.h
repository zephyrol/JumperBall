/*
 * File: Teleporter.h
 * Author: Morgenthaler S
 *
 * Created on 7 novembre 2020, 10:42
 */
#ifndef __TELEPORTER_H__
#define __TELEPORTER_H__

#include "InteractiveSpecial.h"

class Teleporter:public InteractiveSpecial{
public:
    Teleporter(
        const JBTypes::Color& color,
        const JBTypes::Dir& dir,
        const JBTypes::vec3ui& position,
        const Ball_sptr &ball,
        bool isActivated = true
    );

bool isAnimated() const override;
void applySpecialEffect() const override;

};

#endif // __TELEPORTER_H__

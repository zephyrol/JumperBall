/*
 * File: SwitchButton.h
 * Author: Morgenthaler S
 *
 * Created on 7 novembre 2020, 10:42
 */
#ifndef __SWITCHBUTTON_H__
#define __SWITCHBUTTON_H__
#include "InteractiveSpecial.h"

class SwitchButton:public InteractiveSpecial {
public:
SwitchButton(
    const JBTypes::Color& color,
    const JBTypes::Dir& dir,
    const JBTypes::vec3ui& position,
    const Ball_sptr &ball,
    bool isActivated = true
);

void applySpecialEffect() override;
vecCstShape_sptr getShapes() const override;

};

#endif // __SWITCHBUTTON_H__

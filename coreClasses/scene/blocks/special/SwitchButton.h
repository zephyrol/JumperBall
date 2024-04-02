/*
 * File: SwitchButton.h
 * Author: Morgenthaler S
 *
 * Created on 7 novembre 2020, 10:42
 */
#ifndef __SWITCHBUTTON_H__
#define __SWITCHBUTTON_H__

#include "InteractiveSpecial.h"

class SwitchButton : public InteractiveSpecial {
public:
    SwitchButton(
        const JBTypes::Color &color,
        const JBTypes::Dir &dir,
        const glm::u32vec3 &position,
        const Ball_sptr &ball,
        bool isActivated
    );

    DynamicValues <glm::vec3> getDynamicVec3fValues() const override;

    DynamicValues <glm::quat> getDynamicQuaternionValues() const override;

    void applySpecialEffect() override;

    vecCstShape_sptr getShapes() const override;

private:

};

#endif // __SWITCHBUTTON_H__

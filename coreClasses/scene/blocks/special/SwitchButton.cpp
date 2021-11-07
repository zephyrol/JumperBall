/*
 * File: SwitchButton.cpp
 * Author: Morgenthaler S
 *
 * Created on 7 novembre 2020, 10:42
 */
#include "SwitchButton.h"

SwitchButton::SwitchButton(const JBTypes::Color& color,
                           const JBTypes::Dir& dir,
                           const JBTypes::vec3ui& position,
                           const Ball_sptr &ball,
                           bool isActivated
                           ):InteractiveSpecial(color, position, dir, ball, isActivated) {
}

void SwitchButton::applySpecialEffect() const {

}

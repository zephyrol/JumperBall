/*
 * File: SwitchButton.cpp
 * Author: Morgenthaler S
 *
 * Created on 7 novembre 2020, 10:42
 */
#include "SwitchButton.h"

SwitchButton::SwitchButton(const JBTypes::Color& color,
                           const Block& tieBlock,
                           const JBTypes::Dir& dir,
                           const JBTypes::vec3ui& position
                           ):Special(color, tieBlock, dir, position) {
}

Special::SpecialEffect SwitchButton::getEffect() const {
    return SpecialEffect::Switch;
}

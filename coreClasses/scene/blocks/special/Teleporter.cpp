/*
 * File: Teleporter.cpp
 * Author: Morgenthaler S
 *
 * Created on 7 novembre 2020, 10:42
 */
#include "Teleporter.h"

Teleporter::Teleporter(const JBTypes::Color &color, const JBTypes::Dir &dir, const JBTypes::vec3ui &position,
                       const Ball_sptr &ball, bool isActivated):
    InteractiveSpecial(color, position, dir, ball, isActivated){

}

bool Teleporter::isAnimated() const {
    return true;
}

void Teleporter::applySpecialEffect() const {
}


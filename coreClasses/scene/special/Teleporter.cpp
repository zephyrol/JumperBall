/*
 * File: Teleporter.cpp
 * Author: Morgenthaler S
 *
 * Created on 7 novembre 2020, 10:42
 */
#include "Teleporter.h"

Teleporter::Teleporter(const JBTypes::Color& color,
                       const Block& tieBlock,
                       const JBTypes::Dir& dir,
                       const JBTypes::vec3ui& position
                       )
    :Special(color, tieBlock, dir, position) {
}

bool Teleporter::isAnimated() const {
    return true;
}

Special::SpecialEffect Teleporter::getEffect() const {
    return SpecialEffect::Teleportation;
}

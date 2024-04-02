/*
 * File: Bonus.cpp
 * Author: Morgenthaler S
 *
 * Created on 27 mars 2020, 20:48
 */

#include "Bonus.h"

Bonus::Bonus(
    const glm::u32vec3 &position,
    const JBTypes::Dir &direction,
    unsigned int value,
    const Ball_sptr &ball
) :
    InteractiveItem(position, direction, ball),
    _value(value) {
}

unsigned int Bonus::value() const {
    return _value;
}

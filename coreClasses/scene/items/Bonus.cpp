/*
 * File: Bonus.cpp
 * Author: Morgenthaler S
 *
 * Created on 27 mars 2020, 20:48
 */

#include "Bonus.h"

Bonus::Bonus(
    const JBTypes::vec3ui& position,
    const JBTypes::Dir& direction,
    const Item::CategoryOfItems& category,
    unsigned int value):
    Item(position, direction,  category),
    _value(value) {
}

Bonus::~Bonus() {

}

unsigned int Bonus::value() const {
    return _value;
}

/*
 * File: Coin.cpp
 * Author: Morgenthaler S
 *
 * Created on 27 mars 2020, 20:49
 */

#include "Coin.h"

Coin::Coin(const JBTypes::vec3ui& position, const JBTypes::Dir& direction):
    Bonus(position, direction, Item::CategoryOfItems::Coin, 100) {
}

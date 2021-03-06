/*
 * File: Clock.cpp
 * Author: Morgenthaler S
 *
 * Created on 27 mars 2020, 20:48
 */

#include "Clock.h"

Clock::Clock(const JBTypes::vec3ui& position, const JBTypes::Dir& direction):
    Item(position, direction, Item::CategoryOfItems::Clock) {
}

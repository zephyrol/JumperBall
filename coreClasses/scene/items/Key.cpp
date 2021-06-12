/*
 * File: Key.cpp
 * Author: Morgenthaler S
 *
 * Created on 27 mars 2020, 20:45
 */

#include "Key.h"

Key::Key(const JBTypes::vec3ui& position, const JBTypes::Dir& direction):
    Item(position, direction, Item::CategoryOfItems::Key) {
}

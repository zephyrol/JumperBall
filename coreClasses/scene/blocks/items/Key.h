/*
 * File: Key.h
 * Author: Morgenthaler S
 *
 * Created on 27 mars 2020, 20:45
 */

#ifndef KEY_H
#define KEY_H
#include "Item.h"

class Key:public Item {
public:
Key(
    const JBTypes::vec3ui& position,
    const JBTypes::Dir& direction
    );

std::vector<Shape> getShapes() const override;

private:
};

#endif /* KEY_H */

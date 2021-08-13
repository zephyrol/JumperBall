/*
 * File: Bonus.h
 * Author: Morgenthaler S
 *
 * Created on 27 mars 2020, 20:48
 */

#ifndef BONUS_H
#define BONUS_H
#include "Item.h"

class Bonus:public Item {
public:
Bonus(
    const JBTypes::vec3ui& position,
    const JBTypes::Dir& direction,
    unsigned int value
    );

unsigned int value() const;

virtual ~Bonus() = 0;
private:
unsigned int _value;

};

#endif /* BONUS_H */

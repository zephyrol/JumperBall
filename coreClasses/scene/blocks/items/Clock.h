/*
 * File: Clock.h
 * Author: Morgenthaler S
 *
 * Created on 27 mars 2020, 20:48
 */

#ifndef CLOCK_H
#define CLOCK_H
#include "Item.h"

class Clock:public Item {
public:
Clock(const JBTypes::vec3ui& position, const JBTypes::Dir& direction);
vecCstShape_sptr getShapes() const override;

private:

};

#endif /* CLOCK_H */

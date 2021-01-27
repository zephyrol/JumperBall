
/* 
 * File:   Bonus.cpp
 * Author: Morgenthaler S
 * 
 * Created on 27 mars 2020, 20:48
 */

#include "Bonus.h"

Bonus::Bonus( const Object::CategoryOfObjects& category,
              unsigned int value):
Object(category),
_value(value)
{
}

Bonus::~Bonus() {

}

unsigned int Bonus::value() const {
    return _value;
}



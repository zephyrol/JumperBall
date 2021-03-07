/*
 * File: Key.h
 * Author: Morgenthaler S
 *
 * Created on 27 mars 2020, 20:45
 */

#ifndef KEY_H
#define KEY_H
#include "Object.h"

class Key:public Object {
public:
Key(
    const JBTypes::vec3ui& position,
    const JBTypes::Dir& direction
    );

private:
};

#endif /* KEY_H */

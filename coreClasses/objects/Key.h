/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Key.h
 * Author: Morgenthaler S
 *
 * Created on 27 mars 2020, 20:45
 */

#ifndef KEY_H
#define KEY_H
#include "Object.h"

class Key : public Object{
public:
    Key();
    Object::CategoryOfObjects getCategory() override;

private:

};

#endif /* KEY_H */


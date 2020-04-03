/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Object.h
 * Author: Morgenthaler S
 *
 * Created on 27 mars 2020, 19:40
 */

#ifndef OBJECT_H
#define OBJECT_H
#include "../Types.h"

class Object {
public:
    Object();

    virtual ~Object() = default;

    enum class CategoryOfObjects {Key,Coin,Clock};

    virtual CategoryOfObjects getCategory() const = 0;

private:
    bool                                              _gotten;
};

#endif /* OBJECT_H */


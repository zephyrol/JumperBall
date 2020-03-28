/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Clock.h
 * Author: Morgenthaler S
 *
 * Created on 27 mars 2020, 20:48
 */

#ifndef CLOCK_H
#define CLOCK_H
#include "Object.h"

class Clock : public Object{
public:
    Clock();
    Object::CategoryOfObjects getCategory() override;

    
private:

};

#endif /* CLOCK_H */


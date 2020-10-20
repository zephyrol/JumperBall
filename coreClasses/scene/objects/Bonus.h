/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Bonus.h
 * Author: Morgenthaler S
 *
 * Created on 27 mars 2020, 20:48
 */

#ifndef BONUS_H
#define BONUS_H
#include "Object.h" 

class Bonus : public Object{
public:
    Bonus(const CategoryOfObjects &category, unsigned int value);
    
    unsigned int value() const;

    virtual ~Bonus() = 0;
private:
    unsigned int _value;

};

#endif /* BONUS_H */

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Object.cpp
 * Author: Morgenthaler S
 * 
 * Created on 27 mars 2020, 19:40
 */

#include "Object.h"
#include <math.h>

Object::Object(): 
_gotten(false)
{
}

bool Object::isGotten() const { 
    return _gotten;
}

void Object::catchingTest(const JBTypes::vec3f& objectPosition, 
                       const JBTypes::vec3f& entityPosition,
                       float radiusEntity) {
    const float distance = sqrtf(
        powf(objectPosition.x - entityPosition.x,2.f) +
        powf(objectPosition.y - entityPosition.y,2.f) +
        powf(objectPosition.z - entityPosition.z,2.f));
    if (distance < radiusEntity + radiusBoundingSphere) {
    _gotten = true;
    }
    
}



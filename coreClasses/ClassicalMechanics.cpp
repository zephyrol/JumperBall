/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ClassicalMechanics.cpp
 * Author: seb
 * 
 * Created on 17 octobre 2019, 22:14
 */

#include "ClassicalMechanics.h"
#include <math.h>

ClassicalMechanics::ClassicalMechanics():
    _acceleration{0.f,gravitationalAcceleration},
    _directionGravity(ballJumperTypes::Direction::Down),
    _currentEulerMethodBuffer{}
{
}

ClassicalMechanics::ClassicalMechanics(const ballJumperTypes::Direction& dirGravity):
    _acceleration{0.f,gravitationalAcceleration},
    _directionGravity(dirGravity),
    _currentEulerMethodBuffer{}
{
}

ClassicalMechanics::ClassicalMechanics(const float& gAcceleration):
    _acceleration{0.f,gAcceleration},
    _directionGravity(ballJumperTypes::Direction::Down),
    _currentEulerMethodBuffer{}
{
}

ClassicalMechanics::ClassicalMechanics(const ballJumperTypes::Direction& dirGravity,
        const float& gAcceleration):
    _acceleration{0.f,gAcceleration},
    _directionGravity(dirGravity),
    _currentEulerMethodBuffer{}
{
}

ClassicalMechanics::~ClassicalMechanics() {
}

float ClassicalMechanics::getAccelerationX(const float t) const {
    return _acceleration.x + t*0.f;
}

float ClassicalMechanics::getAccelerationY(const float t) const {
    return _acceleration.y + t*0.f;
}

const ClassicalMechanics::physics2DVector ClassicalMechanics::getAcceleration(const float t) const {
    return {getAccelerationX(t),getAccelerationY(t)};
}

float ClassicalMechanics::getVelocityX(const float t, const physics2DVector& v0) const {
    return getAccelerationX(t) * t + v0.x;
}

float ClassicalMechanics::getVelocityY(const float t, const physics2DVector& v0) const {
    return getAccelerationY(t) * t + v0.y;
}

const ClassicalMechanics::physics2DVector ClassicalMechanics::getVelocity(const float t, 
        const physics2DVector& v0) const {
    return {getVelocityX(t,v0), getVelocityY(t,v0)};
}

const ClassicalMechanics::physics2DVector ClassicalMechanics::getVelocity(const float t, 
        const float alpha, const float v0Norm) const {
    const physics2DVector v0 {cos(alpha) * v0Norm, sin(alpha) * v0Norm};
    return getVelocity(t,v0);
}

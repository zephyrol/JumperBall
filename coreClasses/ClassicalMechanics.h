/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ClassicalMechanics.h
 * Author: seb
 *
 * Created on 17 octobre 2019, 22:14
 */

#ifndef CLASSICALMECHANICS_H
#define CLASSICALMECHANICS_H
#include "Types.h"

class ClassicalMechanics {
public:

    //---CONSTRUCTORS---//
    ClassicalMechanics();
    ClassicalMechanics(const ballJumperTypes::Direction& dirGravity);
    ClassicalMechanics(const float& gAcceleration);
    ClassicalMechanics(const ballJumperTypes::Direction& dirGravity,
            const float& gAcceleration);
    virtual ~ClassicalMechanics();
  

    //---CONSTANTS---//
    static constexpr float gravitationalAcceleration = 9.81f;

    //---STRUCTURES---//
    struct physics2DVector { float x; float y; };

    struct EulerMethodBuffer { const float delta {1.f};
                               const struct physics2DVector a0 {0.f,0.f};
                               const struct physics2DVector v0 {0.f,0.f};
    };

    //---METHODS ---//
    const physics2DVector getAcceleration(const float t) const; 
    const physics2DVector getVelocity(const float t, const physics2DVector& v0)
                                                                          const;
    const physics2DVector getVelocity(const float t, const float alpha,
                                      const float v0Norm) const;
    const physics2DVector getPosition(const float t, const physics2DVector& v0)
                                                                          const;
    const physics2DVector getPosition(const float t, const float alpha,
                                      const float v0Norm) const;

private:

    physics2DVector _acceleration;
    ballJumperTypes::Direction _directionGravity;

    mutable EulerMethodBuffer _currentEulerMethodBuffer;

    float getAccelerationX(const float t) const;
    float getAccelerationY(const float t) const;

    float getVelocityX(const float t, const physics2DVector& v0) const;
    float getVelocityY(const float t, const physics2DVector& v0) const;
     
    float gePositionX(const float t, const physics2DVector& v0) const;
    float getPositionY(const float t, const physics2DVector& v0) const;

};

#endif /* CLASSICALMECHANICS_H */


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
#include <functional>

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
    static constexpr float gravitationalAccelerationEarth = 9.81f;
    static constexpr float coefficientWind = 3.f/2.f;
    static constexpr float distanceJumpBasic = 2.f;
    static constexpr float timeToStopWindBasic = 1.f;

    static constexpr size_t sizeSampleEuler= 512;

    //---STRUCTURES---//
    struct physics2DVector { const float x; const float y; };

    struct EulerMethodBufferWind { 
                               float deltaT ;
                               float timeEndWind;
                               float xEndWind;
                               //function to get dx/dt with x and K
                               std::function<float(const float&, 
                               float&)> applicationFunction;
                               std::vector<float> tBuffer;
                               std::vector<float> vBuffer;
                               std::vector<float> pBuffer;
    };

    //---METHODS---//
    float getGravitationalAcceleration() const; 
    const physics2DVector getVelocity(const float t, const physics2DVector& v0)
                                      const;
    const physics2DVector getVelocity(const float t, const float alpha,
                                      const float v0Norm) const;
    const physics2DVector getPosition(const float t, const physics2DVector& v0)
                                      const;
    const physics2DVector getPosition(const float t, const float alpha,
                                      const float v0Norm) const;

    static void solveDifferentialEquationWind (
                                        float& resultDerivativeFunction, 
                                        const float& x, 
                                        const float& K, 
                                        std::function<float(const float&, 
                                        const float&)>& unknownFunction
                                      );

private:

    //---MEMBERS---//
    float _gravitationalAcceleration;
    float _distanceJump;
    float _timeToGetDestinationX;

    ballJumperTypes::Direction _directionGravity;

    mutable EulerMethodBufferWind _EulerMethodBuffer;

    //---METHODS---//

    float getVelocityX(const float t, const physics2DVector& v0) const;
    float getVelocityY(const float t, const physics2DVector& v0) const;
     
    float getPositionX(const float t) const;
    float getPositionY(const float t, const physics2DVector& v0) const;

    void fillEulerBufferWind() const;

};

#endif /* CLASSICALMECHANICS_H */


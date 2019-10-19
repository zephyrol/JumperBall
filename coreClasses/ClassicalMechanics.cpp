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
    _gravitationalAcceleration{gravitationalAccelerationEarth},
    _distanceJump{distanceJumpBasic},
    _timeToGetDestinationX(timeToStopWindBasic),
    _directionGravity(ballJumperTypes::Direction::Down),
    _EulerMethodBuffer{}
{
}

ClassicalMechanics::ClassicalMechanics(
const ballJumperTypes::Direction& dirGravity):
    _gravitationalAcceleration{gravitationalAccelerationEarth},
    _distanceJump{distanceJumpBasic},
    _timeToGetDestinationX(timeToStopWindBasic),
    _directionGravity(dirGravity),
    _EulerMethodBuffer{}
{
}

ClassicalMechanics::ClassicalMechanics(const float& gAcceleration):
    _gravitationalAcceleration{gAcceleration},
    _distanceJump{distanceJumpBasic},
    _timeToGetDestinationX(timeToStopWindBasic),
    _directionGravity(ballJumperTypes::Direction::Down),
    _EulerMethodBuffer{}
{
}

ClassicalMechanics::ClassicalMechanics(
const ballJumperTypes::Direction& dirGravity,
        const float& gAcceleration):
    _gravitationalAcceleration{gAcceleration},
    _distanceJump{distanceJumpBasic},
    _timeToGetDestinationX(timeToStopWindBasic),
    _directionGravity(dirGravity),
    _EulerMethodBuffer{}
{
}

ClassicalMechanics::~ClassicalMechanics() {
}

float ClassicalMechanics::getGravitationalAcceleration() const {
    return _gravitationalAcceleration;
}


float ClassicalMechanics::getVelocityX(const float t, const physics2DVector& v0) 
                                       const {
    return t + v0.x;
}

float ClassicalMechanics::getVelocityY(const float t, const physics2DVector& v0) 
                                       const {
    return _gravitationalAcceleration * t + v0.y;
}

const ClassicalMechanics::physics2DVector ClassicalMechanics::getVelocity(
                                          const float t, 
                                          const physics2DVector& v0) const {
    return {getVelocityX(t,v0), getVelocityY(t,v0)};
}

const ClassicalMechanics::physics2DVector ClassicalMechanics::getVelocity(
                                                            const float t, 
                                                            const float alpha,
                                                            const float v0Norm) 
                                                            const {
    const physics2DVector v0 {cos(alpha) * v0Norm, sin(alpha) * v0Norm};
    return getVelocity(t,v0);
}

float ClassicalMechanics::getPositionX(const float t)
                                       const {
    float posX;
    fillEulerBufferWind();
    if ( t < _EulerMethodBuffer.timeEndWind) {
    float numberElementEuler= t * 
                          static_cast<float>(_EulerMethodBuffer.tBuffer.size())
                          / _EulerMethodBuffer.xEndWind;
    posX = _EulerMethodBuffer.pBuffer.at(
                          static_cast<unsigned int>(numberElementEuler));
    }
    else posX = _EulerMethodBuffer.xEndWind;
    return posX;
}

float ClassicalMechanics::getPositionY(const float t, const physics2DVector& v0)
                                       const {
    return (-_gravitationalAcceleration * static_cast<float>(pow(t,2)) / 2.f) 
            + v0.y * t;
}

void ClassicalMechanics::fillEulerBufferWind() const {

  if ( _EulerMethodBuffer.pBuffer.size() != sizeSampleEuler ||
       fabs(_EulerMethodBuffer.xEndWind-_distanceJump) > EPSILON_F ||
       fabs(_EulerMethodBuffer.timeEndWind -_timeToGetDestinationX) 
          > EPSILON_F) {

      const float newXEndWind = _distanceJump;
      const float newTimeEndWind = _timeToGetDestinationX;
      const float newDeltaT= _timeToGetDestinationX / sizeSampleEuler;
      const float coeffWind = coefficientWind;

      std::vector<float> newTBuffer;
      std::vector<float> newVBuffer;
      std::vector<float> newPBuffer;

      newTBuffer.resize(sizeSampleEuler);
      newVBuffer.resize(sizeSampleEuler);
      newPBuffer.resize(sizeSampleEuler);

      newTBuffer.at(sizeSampleEuler-1) = _timeToGetDestinationX;
      newVBuffer.at(sizeSampleEuler-1) = 0.f;
      newPBuffer.at(sizeSampleEuler-1) = _distanceJump;

      
      std::function<float(const float&, const float&)> 
      newApplicationFunction= [] (const float& x, const float& K) -> float {
          return 1.f/(x*K);
      };

      for ( int i = sizeSampleEuler-2 ; i >= 0 ; i-- ) {
        newTBuffer.at(i) = newTBuffer.at(i+1) - newDeltaT;
        newPBuffer.at(i) = newPBuffer.at(i+1) - newDeltaT * newVBuffer[i+1];
        newVBuffer.at(i) = newApplicationFunction(newPBuffer.at(i),
                                                 coeffWind);
        solveDifferentialEquationWind( newVBuffer.at(i), newPBuffer.at(i), 
                                       coeffWind, newApplicationFunction);
                
      }

      EulerMethodBufferWind newEulerTab {newDeltaT,newTimeEndWind, 
                                        newXEndWind,newApplicationFunction,
                                        newTBuffer,newVBuffer,newPBuffer};

     _EulerMethodBuffer = newEulerTab; 
  }
}

void ClassicalMechanics::solveDifferentialEquationWind(
            float& resultDerivativeFunction, 
            const float& x, 
            const float& K, 
            std::function<float(const float&, const float&) >& unknownFunction) 
{
    resultDerivativeFunction = unknownFunction(x,K);
}




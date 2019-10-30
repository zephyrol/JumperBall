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
#include <iostream>
#include <initializer_list>


const std::map<ClassicalMechanics::Fluid,float>
ClassicalMechanics::listOfViscosities  {
                                {ClassicalMechanics::Fluid::Air,       1.8e-5f},
                                {ClassicalMechanics::Fluid::Water,   1.002e-3f},
                                {ClassicalMechanics::Fluid::IcyWater,1.787e-3f},
                                {ClassicalMechanics::Fluid::Oil,      56.2e-3f}
};

ClassicalMechanics::ClassicalMechanics():
    _gravitationalAcceleration{gravitationalAccelerationEarth},
    _distanceJump{distanceJumpBasic},
    _timeToGetDestinationX(timeToStopWindBasic),
    _weightSphere(0.010f),
    _v0{getV0xToRespectDistanceAndTime(),1.f},
    _fluid(ClassicalMechanics::Fluid::Air),
    _EulerMethodBuffer{}
{
}

ClassicalMechanics::~ClassicalMechanics() {
}

float ClassicalMechanics::getGravitationalAcceleration() const {
    return _gravitationalAcceleration;
}

const ClassicalMechanics::physics2DVector ClassicalMechanics::getPosition(
                                          const float t) const {
    return {getPositionX(t), getPositionY(t)};
}

const ClassicalMechanics::physics2DVector ClassicalMechanics::getVelocity(
                                          const float t) const {
    return {getVelocityX(t), getVelocityY(t)};
}

const ClassicalMechanics::physics2DVector ClassicalMechanics::getAcceleration(
                                          const float t) const {
    return {getAccelerationX(t), getAccelerationY(t)};
}


float ClassicalMechanics::getPositionX(const float t) const {
    float posX;
    if ( t < _timeToGetDestinationX) {
    posX = _v0.x * _timeToGetDestinationX * t - _v0.x * 
       static_cast<float> (pow(t,2)) / 2.f ;
    }
    else 
        posX = _distanceJump;
    return posX;
}

float ClassicalMechanics::getPositionY(const float t) const {
    fillEulerMethodBuffer();
    return _EulerMethodBuffer.pBuffer.at(
            static_cast<unsigned int> (t * sizeSampleEuler / durationStudy ));
}


float ClassicalMechanics::getVelocityX(const float t) const {
    float velocityX;
    if ( t < _timeToGetDestinationX)
    velocityX = _v0.x * ( _timeToGetDestinationX - t );
    else velocityX = 0.f;
    return velocityX;
}

float ClassicalMechanics::getVelocityY(const float t) const {
    fillEulerMethodBuffer();
    return _EulerMethodBuffer.vBuffer.at(
            static_cast<unsigned int> (t * sizeSampleEuler / durationStudy ));
}

float ClassicalMechanics::getAccelerationX(const float t) const {
    float accelerationX;
    if ( t < _timeToGetDestinationX)
        accelerationX =  -_v0.x;
    else
        accelerationX = 0.f;

    return accelerationX;
}

float ClassicalMechanics::getAccelerationY(const float t) const {
    fillEulerMethodBuffer();
    return _EulerMethodBuffer.aBuffer.at(
            static_cast<unsigned int> (t * sizeSampleEuler / durationStudy ));
}

void ClassicalMechanics::fillEulerMethodBuffer() const {

  if ( _EulerMethodBuffer.pBuffer.size() != sizeSampleEuler ||
       _EulerMethodBuffer.deltaT - durationStudy/sizeSampleEuler > EPSILON_F ||
       _EulerMethodBuffer.fluid != _fluid) {

      const float newDeltaT= durationStudy / sizeSampleEuler;

      std::vector<float> newTBuffer (sizeSampleEuler);
      std::vector<float> newPBuffer (sizeSampleEuler);
      std::vector<float> newVBuffer (sizeSampleEuler);
      std::vector<float> newABuffer (sizeSampleEuler);

      newTBuffer.at(0) = 0.f;
      newPBuffer.at(0) = 0.f;
      newVBuffer.at(0) = 0.f;
      
      const std::function<float(const std::array<float,5>&)> NewtonStokesFunc =
      [] (std::array<float,5> params) -> float {
          
          const float velocity                  = params.at(0);
          const float radiusSphere              = params.at(1);
          const float viscosity                 = params.at(2);
          const float gravitationalAcceleration = params.at(3);
          const float weightSphere              = params.at(4);

          const float gravityComponent= weightSphere* gravitationalAcceleration;
          const float StokesComponent =  6.f * static_cast<float>(M_PI) *
                                        viscosity * radiusSphere * 
                                         static_cast<float>(pow(velocity,2));

          const float sumForces     = gravityComponent - StokesComponent; 
          const float acceleration  = - sumForces / weightSphere; 

          return acceleration;
      };

          const std::array<float,5> paramsInitials  { newVBuffer.at(0), 
                                                  radiusBall,
                                                  listOfViscosities.at(_fluid), 
                                                  _gravitationalAcceleration,
                                                  _weightSphere
                                                };
          solveDifferentialEquation (newABuffer.at(0),
                                     NewtonStokesFunc, 
                                     paramsInitials);

      for ( unsigned int i = 0 ; i < sizeSampleEuler-1 ; i++) {


          newTBuffer.at(i+1) = newTBuffer.at(i) + newDeltaT ;
          
          newPBuffer.at(i+1) = newPBuffer.at(i) + newDeltaT * newVBuffer.at(i);

          newVBuffer.at(i+1) = newVBuffer.at(i) + newDeltaT * newABuffer.at(i);

          const std::array<float,5> parameters  { newVBuffer.at(i+1), 
                                                  radiusBall,
                                                  listOfViscosities.at(_fluid), 
                                                  _gravitationalAcceleration,
                                                  _weightSphere
                                                };
          
          solveDifferentialEquation( newABuffer.at(i+1), NewtonStokesFunc, 
                                     parameters);
      }

      EulerMethodBuffer newEulerTab { newDeltaT, _fluid, NewtonStokesFunc,
                                      newTBuffer, newABuffer, newVBuffer,
                                      newPBuffer };

      _EulerMethodBuffer = newEulerTab; 
  }
}


void ClassicalMechanics::solveDifferentialEquation(
      float& derivative, 
      const std::function<float(const std::array<float,5>&)>& computingFunction,
      const std::array<float, 5>& params)
{
    derivative = computingFunction ( params );
}


void ClassicalMechanics::printEulerBuffer() const {

    fillEulerMethodBuffer();
    for (unsigned int i = 0 ; i < _EulerMethodBuffer.tBuffer.size(); i++)  {
      std::cout << "t = " << _EulerMethodBuffer.tBuffer.at(i) << "\tp = "
              << _EulerMethodBuffer.pBuffer.at(i) << "\tv = "
              << _EulerMethodBuffer.vBuffer.at(i) << "\ta = "
              << _EulerMethodBuffer.aBuffer.at(i) << "\tX = "
              << getPositionX(_EulerMethodBuffer.tBuffer.at(i)) << std::endl;
    }
}

float ClassicalMechanics::getV0xToRespectDistanceAndTime() {
    return 2.f*_distanceJump/ static_cast<float>(pow(_timeToGetDestinationX,2));

}

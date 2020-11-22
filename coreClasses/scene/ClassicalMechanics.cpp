/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ClassicalMechanics.cpp
 * Author: Morgenthaler S
 * 
 * Created on 17 octobre 2019, 22:14
 */

#include "ClassicalMechanics.h"
#include <math.h>
#include <iostream>


const std::map<ClassicalMechanics::Fluid,float>
ClassicalMechanics::listOfViscosities  {
                                {ClassicalMechanics::Fluid::Air,       1.8e-5f},
                                {ClassicalMechanics::Fluid::Water,   1.002e-3f},
                                {ClassicalMechanics::Fluid::IcyWater,1.787e-3f},
                                {ClassicalMechanics::Fluid::Oil,      56.2e-3f}
};

ClassicalMechanics::ClassicalMechanics():
    _gravitationalAcceleration          {gravitationalAccelerationEarth},
    _jumpDistance                       {basicJumpDistance},
    _timeToGetDestinationX              (timeToStopWindBasic),
    _weightSphere                       (0.010f),
    _v0                                 {getV0xToRespectDistanceAndTime(),3.f},
    _fluid                              (ClassicalMechanics::Fluid::Air),
    _EulerMethodBuffer                  {},
    _timesShock                         {}
{
    fillEulerMethodBuffer();
}

ClassicalMechanics::ClassicalMechanics( float jumpDistance, 
                                        float timeToGetDestination, 
                                        float v0y):
    _gravitationalAcceleration          {gravitationalAccelerationEarth},
    _jumpDistance                       {jumpDistance},
    _timeToGetDestinationX              (timeToGetDestination),
    _weightSphere                       (0.010f),
    _v0                                 (ClassicalMechanics::physics2DVector
                                        {getV0xToRespectDistanceAndTime(),v0y}),
    _fluid                              (ClassicalMechanics::Fluid::Air),
    _EulerMethodBuffer                  {},
    _timesShock                         {}
{
    fillEulerMethodBuffer();
}

float ClassicalMechanics::getJumpDistance() const {
    return _jumpDistance;
}

float ClassicalMechanics::getGravitationalAcceleration() const {
    return _gravitationalAcceleration;
}

ClassicalMechanics::physics2DVector ClassicalMechanics::getPosition(
                                          const float t) const {
    return {getPositionX(t), getPositionY(t)};
}

ClassicalMechanics::physics2DVector ClassicalMechanics::getVelocity(
                                          const float t) const {
    return {getVelocityX(t), getVelocityY(t)};
}

ClassicalMechanics::physics2DVector ClassicalMechanics::getAcceleration(
                                          const float t) const {
    return {getAccelerationX(t), getAccelerationY(t)};
}

float ClassicalMechanics::evalPositionX(const float t) const {
    const float posX = t < _timeToGetDestinationX
        ? _v0.x * _timeToGetDestinationX * t - _v0.x * 
            static_cast<float> (pow(t,2)) / 2.f
        :_jumpDistance;
    return posX;
}

float ClassicalMechanics::evalTimeFromPosX(const float x) const {
    const std::pair<float, float> solutions =
        solveQuadraticEquation(-_v0.x / 2.f,
                               _v0.x * _timeToGetDestinationX,
                               -x);

    const auto getTime = [](const std::pair<float, float> &solutions) -> float {
        float t;
        if (solutions.first >= 0.f) {
            t = solutions.first;
        }
        else if (solutions.second >= 0.f) {
            t = solutions.second;
        }
        else {
            std::cerr << " Error : negative value found about the time ... "
                      << " 0 is return ... "
                      << std::endl;
            t = 0.f;
        }

        return t;
    };

    const float time = getTime(solutions);
    return time;
}

float ClassicalMechanics::getIntervalX(float tBegin, float tEnd) const {
    return evalPositionX(tEnd) - evalPositionX(tBegin);
}

float ClassicalMechanics::getPositionX( const float t ) const {
    float posX;
     
    if (_timesShock.empty()){
      posX = evalPositionX(t) ;
    } else {
        std::vector<float> intervalsTimes { 0.f };
      
        for ( float time : _timesShock) {
            if (t > time) {
                const float t1 = time;
                const float t2 = evalTimeFromPosX(evalPositionX(t1) +
                                                  2.f * radiusBall);
                intervalsTimes.push_back(t1);
                intervalsTimes.push_back(t2);
                intervalsTimes.push_back(t2 + (t - t1));
            } else {
                intervalsTimes.push_back(t);
            }
        }
        
        posX = 0;
        float sign = 1.f;
        while(intervalsTimes.size() > 0) {
            posX += sign * getIntervalX( intervalsTimes.at(0),
                                         intervalsTimes.at(1));
          
            intervalsTimes.erase  ( intervalsTimes.begin(),
                                    intervalsTimes.begin()+2
                                  );
            sign *= -1.f;
        }
         
    }

    return posX;
}

float ClassicalMechanics::getPositionY(const float t) const {
    fillEulerMethodBuffer();

    return _EulerMethodBuffer.pBuffer.at(getIndexEulerBuffer(t));
}

float ClassicalMechanics::getVelocityX(const float t) const {
    const float velocityX = t < _timeToGetDestinationX
        ? _v0.x * ( _timeToGetDestinationX - t )
        : 0.f;
    return velocityX;
}

float ClassicalMechanics::getVelocityY(const float t) const {
    fillEulerMethodBuffer();
    return _EulerMethodBuffer.vBuffer.at(getIndexEulerBuffer(t));
}

float ClassicalMechanics::getAccelerationX(const float t) const {
    const float accelerationX = t < _timeToGetDestinationX 
        ? -_v0.x
        : 0.f;
    return accelerationX;
}

float ClassicalMechanics::getAccelerationY(const float t) const {
    fillEulerMethodBuffer();
    return _EulerMethodBuffer.aBuffer.at(getIndexEulerBuffer(t));
}

float ClassicalMechanics::getTimeToGetDestination() const {
    return _timeToGetDestinationX;
 }

void ClassicalMechanics::fillEulerMethodBuffer() const {

  if ( _EulerMethodBuffer.pBuffer.size() != sizeSampleEuler ||
       _EulerMethodBuffer.deltaT - durationStudy/sizeSampleEuler > EPSILON_F ||
       _EulerMethodBuffer.fluid != _fluid) {

      const float newDeltaT = durationStudy / sizeSampleEuler;

      std::vector<float> newTBuffer (sizeSampleEuler);
      std::vector<float> newPBuffer (sizeSampleEuler);
      std::vector<float> newVBuffer (sizeSampleEuler);
      std::vector<float> newABuffer (sizeSampleEuler);

      newTBuffer.at(0) = 0.f;
      newPBuffer.at(0) = 0.f;
      newVBuffer.at(0) = _v0.y;
      
      const std::function<float(const std::vector<float>&)> NewtonStokesFunc =
      [] (const std::vector<float>& params) -> float {
          
          constexpr size_t numberParamsNewtonStokesFunc = 5;
          float acceleration;

          if (params.size() != numberParamsNewtonStokesFunc ) {
              std::cerr << " Error : Bad number of parameters to compute "
                        << " the vertical acceleration ... 0 is returned " 
                        << std::endl;
              acceleration = 0.f;
          }
          else {
              const float velocity                  = params.at(0);
              const float radiusSphere              = params.at(1);
              const float viscosity                 = params.at(2);
              const float gravitationalAcceleration = params.at(3);
              const float weightSphere              = params.at(4);
              
              const float gravityComponent= weightSphere * 
                                              gravitationalAcceleration;
              const float StokesComponent = 6.f * static_cast<float>(M_PI) *
                                            viscosity * radiusSphere * 
                                            static_cast<float>(pow(velocity,2));
              
              const float sumForces       = (-gravityComponent) + 
                                              StokesComponent; 
              acceleration  = sumForces / weightSphere; 
          }
          
          return acceleration;
      };

      const std::vector<float> paramsInitials   { newVBuffer.at(0), 
                                                  radiusBall,
                                                  listOfViscosities.at(_fluid), 
                                                  _gravitationalAcceleration,
                                                  _weightSphere
                                                };
      solveDifferentialEquation (newABuffer.at(0),
                                NewtonStokesFunc, 
                                paramsInitials);

      for ( size_t i = 0 ; i < sizeSampleEuler-1 ; i++) {


          newTBuffer.at(i+1) = newTBuffer.at(i) + newDeltaT ;
          
          newPBuffer.at(i+1) = newPBuffer.at(i) + newDeltaT * newVBuffer.at(i);

          newVBuffer.at(i+1) = newVBuffer.at(i) + newDeltaT * newABuffer.at(i);

          const std::vector<float> parameters   { newVBuffer.at(i+1), 
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

      _EulerMethodBuffer = std::move(newEulerTab); 
  }
}

float ClassicalMechanics::getIndexEulerBuffer(float t) const {
  
    const unsigned int index = t < durationStudy
        ? static_cast<unsigned int> (t * sizeSampleEuler / durationStudy)
        : static_cast<unsigned int>(durationStudy-1);

    return index;
}

void ClassicalMechanics::solveDifferentialEquation(
      float& derivative, 
      const std::function<float(const std::vector<float>&)>& computingFunction,
      const std::vector<float>& params)
{
    derivative = computingFunction(params);
}

 std::pair<float, float> ClassicalMechanics::solveQuadraticEquation(
    float a, float b, float c) 
{
    if ( a < EPSILON_F && a > -EPSILON_F) {
        std::cerr << "Error: Trying to divide by 0 ... " << 
                "solutions cropped to 0... " 
                << std::endl ;
        return { 0.f , 0.f };
    }
    const float delta = static_cast<float> (pow(b, 2.)) - 4.f * a * c;
    if (delta < 0.f) {
        std::cerr << "Error: Non-real solutions ... : cropped to 0... " 
                << std::endl ;
        return { 0.f , 0.f };
    } else {
        return {
            (-b + sqrtf(delta)) / (2.f * a),
            (-b - sqrtf(delta)) / (2.f * a)
            };
    }
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

float ClassicalMechanics::getV0xToRespectDistanceAndTime() const {
    return 2.f*_jumpDistance/ static_cast<float>(pow(_timeToGetDestinationX,2));
}

const std::vector<float>& ClassicalMechanics::timesShock() {
    return _timesShock;
}

void ClassicalMechanics::timesShock(const std::vector<float>& v) {
    _timesShock = v;
}

void ClassicalMechanics::addShockFromPosition(float pos) {
    _timesShock.push_back(evalTimeFromPosX(pos));
}

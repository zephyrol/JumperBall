/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#ifndef CLASSICALMECHANICS_H
#define CLASSICALMECHANICS_H


/* 
 * File:   ClassicalMechanics.h
 * Author: Morgenthaler S
 *
 * Created on 17 octobre 2019, 22:14
 */

#include "system/Types.h"
#include <functional>
#include <map>
#include <array>
#include <vector>

class ClassicalMechanics {
public:

    //--CONSTRUCTORS & DESTRUCTORS--//
    ClassicalMechanics                  ();

    ClassicalMechanics                  ( float distanceJump,
                                          float timeToGetDestination,
                                          float v0y );

  
    //---------CONSTANTS------------//
    static constexpr float              gravitationalAccelerationEarth = 9.81f;
    static constexpr float              distanceJumpBasic              = 2.f;
    static constexpr float              timeToStopWindBasic            = 0.7f;

    static constexpr size_t             sizeSampleEuler                = 32768;
    static constexpr float              durationStudy                  = 10.f;
    static constexpr float              radiusBall                     = 0.2f;


    //------------TYPES------------//
    enum class Fluid                    { Air,Water,IcyWater,Oil };
    
    static const std::map<Fluid,float>  listOfViscosities;

    struct physics2DVector              { float x; float y; };

    struct EulerMethodBuffer            { float              deltaT ;
                                          Fluid              fluid;
                                          std::function <
                                            float(const std::vector<float>&)
                                                        >    computingFunction;
                                          std::vector<float> tBuffer;
                                          std::vector<float> aBuffer;
                                          std::vector<float> vBuffer;
                                          std::vector<float> pBuffer; };


    //-------CONST METHODS--------//
    float                               getGravitationalAcceleration()    const; 
    physics2DVector                     getAcceleration(const float t)    const;
    physics2DVector                     getVelocity(const float t)        const;
    physics2DVector                     getPosition(const float t)        const;
    float                               getTimeToGetDestination()         const;
    void                                printEulerBuffer()                const;


    //--------STATIC METHODS-------//
    static void                         solveDifferentialEquation (
      float& derivative,
      const std::function<float(const std::vector<float>&)>& computingFunction,
      const std::vector<float>& params);

    static void                         solveQuadraticEquation (
                                              std::pair<float,float>& solutions, 
                                              float a, float b, float c);
                                                              

    //----------METHODS------------//
    const std::vector<float>&           timesShock();
    void                                timesShock(const std::vector<float>& v);
    void                                addShockFromPosition(float pos);

private:

    //--------ATTRIBUTES-----------//
    float                               _gravitationalAcceleration;
    float                               _distanceJump;
    float                               _timeToGetDestinationX;

    float                               _weightSphere;

    const physics2DVector               _v0;

    const ClassicalMechanics::Fluid     _fluid;

    mutable EulerMethodBuffer           _EulerMethodBuffer;

    std::vector<float>                  _timesShock;

    //-------CONST METHODS--------//
    float                               getVelocityX(const float t)       const;
    float                               getVelocityY(const float t)       const;
     

    float                               getIntervalX(float tBegin, float tEnd)
                                                                          const;
    float                               evalPositionX(const float t)      const;
    float                               getPositionX(const float t)       const;
    float                               evalTimeFromPosX (const float x)  const;

    float                               getPositionY(const float t)       const;

    float                               getAccelerationX(const float t)   const;
    float                               getAccelerationY(const float t)   const;


    void                                fillEulerMethodBuffer()           const;

    float                               getV0xToRespectDistanceAndTime()  const;

};

#endif /* CLASSICALMECHANICS_H */


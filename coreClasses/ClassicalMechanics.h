/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#ifndef CLASSICALMECHANICS_H
#define CLASSICALMECHANICS_H


/* 
 * File:   ClassicalMechanics.h
 * Author: seb
 *
 * Created on 17 octobre 2019, 22:14
 */

#include "Types.h"
#include <functional>
#include <map>
#include <array>
#include <vector>

class ClassicalMechanics {
public:

    //--CONSTRUCTORS & DESTRUCTORS--//
    ClassicalMechanics                  ();
    virtual ~ClassicalMechanics         ();

  
    //---------CONSTANTS------------//
    static constexpr float              gravitationalAccelerationEarth = 9.81f;
    static constexpr float              distanceJumpBasic              = 2.f;
    static constexpr float              timeToStopWindBasic            = 0.7f;

    static constexpr size_t             sizeSampleEuler                = 8192;
    static constexpr float              durationStudy                  = 10.f;
    static constexpr float              radiusBall                     = 0.20f;


    //------------TYPES------------//
    enum class Fluid                    {Air,Water,IcyWater,Oil};
    
    static const std::map<Fluid,float>  listOfViscosities;

    struct physics2DVector              { const float x; const float y; };

    struct EulerMethodBuffer            { 
                                          float              deltaT ;
                                          Fluid              fluid;
                                          std::function <
                                            float(const std::array<float,5>&)
                                                        >    computingFunction;
                                          std::vector<float> tBuffer;
                                          std::vector<float> aBuffer;
                                          std::vector<float> vBuffer;
                                          std::vector<float> pBuffer;
                                        };


    //-------CONST METHODS--------//
    float                               getGravitationalAcceleration()    const; 
    const physics2DVector               getAcceleration(const float t)    const;
    const physics2DVector               getVelocity(const float t)        const;
    const physics2DVector               getPosition(const float t)        const;

    void                                printEulerBuffer()                const;


    //--------STATIC METHODS-------//
    static void                         solveDifferentialEquation (
      float& derivative,
      const std::function<float(const std::array<float,5>&)>& computingFunction,
      const std::array<float,5>& params
                                                                  );

    //----------METHODS------------//
    const std::vector<float>&           timesShock();
    void                                timesShock(const std::vector<float>& v);

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
    float                               getPositionY(const float t)       const;

    float                               getAccelerationX(const float t)   const;
    float                               getAccelerationY(const float t)   const;


    void                                fillEulerMethodBuffer()           const;

    float                               getV0xToRespectDistanceAndTime()  const;

};

#endif /* CLASSICALMECHANICS_H */


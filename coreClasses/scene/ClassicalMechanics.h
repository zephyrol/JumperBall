
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
    ClassicalMechanics                  (float ballRadius);

    ClassicalMechanics                  (float ballRadius,
                                         float jumpDistance,
                                         float v0y );
  
    //---------CONSTANTS------------//
    static constexpr float              gravitationalAccelerationEarth = 9.81f;
    static constexpr float              basicJumpDistance              = 2.f;
    static constexpr float              basicV0y                       = 3.2f;


    //------------TYPES------------//
    struct physics2DVector              { float x; float y; };


    //-------CONST METHODS--------//
    float                               getJumpDistance()                 const; 
    float                               getGravitationalAcceleration()    const; 
    physics2DVector                     getAcceleration(const float t)    const;
    physics2DVector                     getVelocity(const float t)        const;
    physics2DVector                     getPosition(const float t)        const;
    float                               getTimeToGetDestination()         const;

    //--------STATIC METHODS-------//
    static std::pair<float,float>       solveQuadraticEquation (
                                              float a, float b, float c);
                                                              

    //----------METHODS------------//
    const std::vector<float>&           timesShock();
    void                                timesShock(const std::vector<float>& v);
    void                                addShockFromPosition(float pos);

private:

    //--------ATTRIBUTES-----------//
    float                               _gravitationalAcceleration;
    float                               _jumpDistance;
    float                               _timeToGetDestinationX;
    const physics2DVector               _v0;
    float                               _ballRadius;

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

    float                               getV0xToRespectDistanceAndTime()  const;
    float                               getTimeToGetDestFromV0y(
                                                              float v0y)  const;

};

#endif /* CLASSICALMECHANICS_H */

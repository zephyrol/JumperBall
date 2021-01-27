
/* 
 * File:   ClassicalMechanics.cpp
 * Author: Morgenthaler S
 * 
 * Created on 17 octobre 2019, 22:14
 */

#include "ClassicalMechanics.h"
#include <math.h>
#include <iostream>


ClassicalMechanics::ClassicalMechanics(float ballRadius):
    _gravitationalAcceleration {gravitationalAccelerationEarth},
    _jumpDistance              {basicJumpDistance},
    _timeToGetDestinationX     (getTimeToGetDestFromV0y(basicV0y)),
    _v0                        {getV0xToRespectDistanceAndTime(),basicV0y},
    _ballRadius                (ballRadius),
    _timesShock                {}
{
}

ClassicalMechanics::ClassicalMechanics( float ballRadius,
                                        float jumpDistance,
                                        float v0y):
    _gravitationalAcceleration {gravitationalAccelerationEarth},
    _jumpDistance              {jumpDistance},
    _timeToGetDestinationX     (getTimeToGetDestFromV0y(v0y)),
    _v0                        {getV0xToRespectDistanceAndTime(),v0y},
    _ballRadius                (ballRadius),
    _timesShock                {}
{
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
                                                  2.f * _ballRadius);
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
    return _v0.y * t - powf(t, 2.f) * gravitationalAccelerationEarth / 2.f;
}

float ClassicalMechanics::getVelocityX(const float t) const {
    const float velocityX = t < _timeToGetDestinationX
        ? _v0.x * ( _timeToGetDestinationX - t )
        : 0.f;
    return velocityX;
}

float ClassicalMechanics::getVelocityY(const float t) const {
    return -gravitationalAccelerationEarth * t + _v0.y;
}

float ClassicalMechanics::getAccelerationX(const float t) const {
    const float accelerationX = t < _timeToGetDestinationX 
        ? -_v0.x
        : 0.f;
    return accelerationX;
}

float ClassicalMechanics::getAccelerationY(const float) const {
    return -gravitationalAccelerationEarth;
}

float ClassicalMechanics::getTimeToGetDestination() const {
    return _timeToGetDestinationX;
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

float ClassicalMechanics::getV0xToRespectDistanceAndTime() const {
    return 2.f*_jumpDistance/ static_cast<float>(pow(_timeToGetDestinationX,2));
}

float ClassicalMechanics::getTimeToGetDestFromV0y(float v0y) const {
    const std::pair<float,float> times =
        solveQuadraticEquation(-gravitationalAccelerationEarth/2.f, v0y, 0.f);

    return times.first > 0.f
        ? times.first
        : times.second;
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

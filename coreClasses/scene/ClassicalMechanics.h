#ifndef CLASSICALMECHANICS_H
#define CLASSICALMECHANICS_H

/*
 * File: ClassicalMechanics.h
 * Author: Morgenthaler S
 *
 * Created on 17 octobre 2019, 22:14
 */

#include "system/Types.h"

class ClassicalMechanics {
public:

    explicit ClassicalMechanics(float ballRadius, float speedCoefficient);

    ClassicalMechanics(float ballRadius, float jumpDistance, float v0y, float speedCoefficient);

    static constexpr float gravitationalAccelerationEarth = 9.81f;
    static constexpr float basicJumpDistance = 2.f;
    static constexpr float basicV0y = 3.2f;

    struct physics2DVector {
        float x;
        float y;
    };

    float getJumpDistance() const;

    physics2DVector getVelocity(float t) const;

    physics2DVector getPosition(float t) const;

    float getTimeToGetDestination() const;

    void timesShock(const std::vector<float> &v);

    void addShockFromPosition(float pos);

private:

    const float _jumpDistance;
    const float _timeToGetDestinationX;
    const physics2DVector _v0;
    const float _ballRadius;
    const float _speedCoefficient;

    std::vector<float> _timesShock;

    float getVelocityX(float t) const;

    float getVelocityY(float t) const;

    float getIntervalX(float tBegin, float tEnd) const;

    float evalPositionX(float t) const;

    float getPositionX(float t) const;

    float evalTimeFromPosX(float x) const;

    float getPositionY(float t) const;

    float getV0xToRespectDistanceAndTime() const;

    static float getTimeToGetDestFromV0y(float v0y);

    static std::pair<float, float> solveQuadraticEquation(float a, float b, float c);

};

#endif /* CLASSICALMECHANICS_H */

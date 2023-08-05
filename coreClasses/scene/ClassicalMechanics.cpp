/*
 * File: ClassicalMechanics.cpp
 * Author: Morgenthaler S
 *
 * Created on 17 octobre 2019, 22:14
 */

#include "ClassicalMechanics.h"


ClassicalMechanics::ClassicalMechanics(float ballRadius, float speedCoefficient) :
    _jumpDistance{basicJumpDistance},
    _timeToGetDestinationX(getTimeToGetDestFromV0y(basicV0y)),
    _v0{getV0xToRespectDistanceAndTime(), basicV0y},
    _ballRadius(ballRadius),
    _speedCoefficient(speedCoefficient),
    _timesShock{}
    {
}

ClassicalMechanics::ClassicalMechanics(float ballRadius, float jumpDistance, float v0y, float speedCoefficient) :
    _jumpDistance{jumpDistance},
    _timeToGetDestinationX(getTimeToGetDestFromV0y(v0y)),
    _v0{getV0xToRespectDistanceAndTime(), v0y},
    _ballRadius(ballRadius),
    _speedCoefficient(speedCoefficient),
    _timesShock{}{
}

float ClassicalMechanics::getJumpDistance() const {
    return _jumpDistance;
}

ClassicalMechanics::physics2DVector ClassicalMechanics::getPosition(float t) const {
    return {getPositionX(t * _speedCoefficient), getPositionY(t * _speedCoefficient)};
}

ClassicalMechanics::physics2DVector ClassicalMechanics::getVelocity(float t) const {
    return {getVelocityX(t * _speedCoefficient), getVelocityY(t * _speedCoefficient)};
}

float ClassicalMechanics::evalPositionX(float t) const {
    const float posX = t < _timeToGetDestinationX
                       ? _v0.x * _timeToGetDestinationX * t - _v0.x * t * t / 2.f
                       : _jumpDistance;
    return posX;
}

float ClassicalMechanics::evalTimeFromPosX(float x) const {
    const std::pair<float, float> solutions = solveQuadraticEquation(
        -_v0.x / 2.f,
        _v0.x * _timeToGetDestinationX,
        -x
    );

    const auto getTime = [](const std::pair<float, float> &solutions) -> float {
        float t;
        if (solutions.first >= 0.f) {
            t = solutions.first;
        } else if (solutions.second >= 0.f) {
            t = solutions.second;
        } else {
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

float ClassicalMechanics::getPositionX(float t) const {
    float posX;

    if (_timesShock.empty()) {
        posX = evalPositionX(t);
    } else {
        std::vector<float> intervalsTimes{0.f};

        for (float time: _timesShock) {
            if (t > time) {
                const float t1 = time;
                const float t2 = evalTimeFromPosX(evalPositionX(t1) + 2.f * _ballRadius);
                intervalsTimes.push_back(t1);
                intervalsTimes.push_back(t2);
                intervalsTimes.push_back(t2 + (t - t1));
            } else {
                intervalsTimes.push_back(t);
            }
        }

        posX = 0;
        float sign = 1.f;
        while (!intervalsTimes.empty()) {
            posX += sign * getIntervalX(intervalsTimes.at(0), intervalsTimes.at(1));

            intervalsTimes.erase(intervalsTimes.begin(), intervalsTimes.begin() + 2);
            sign = -sign;
        }
    }

    return posX;
}

float ClassicalMechanics::getPositionY(float t) const {
    return _v0.y * t - t * t * gravitationalAccelerationEarth / 2.f;
}

float ClassicalMechanics::getVelocityX(float t) const {
    const float velocityX = t < _timeToGetDestinationX
                            ? _v0.x * (_timeToGetDestinationX - t)
                            : 0.f;
    return velocityX;
}

float ClassicalMechanics::getVelocityY(float t) const {
    return -gravitationalAccelerationEarth * t + _v0.y;
}

float ClassicalMechanics::getTimeToGetDestination() const {
    return _timeToGetDestinationX / _speedCoefficient;
}

std::pair<float, float> ClassicalMechanics::solveQuadraticEquation(float a, float b, float c) {
    const float delta = b * b - 4.f * a * c;
    const auto twoA = 2.f * a;
    const auto sqrtDelta = sqrtf(delta);
    return {
        (-b + sqrtDelta) / twoA,
        (-b - sqrtDelta) / twoA
    };
}

float ClassicalMechanics::getV0xToRespectDistanceAndTime() const {
    return 2.f * _jumpDistance / (_timeToGetDestinationX * _timeToGetDestinationX);
}

float ClassicalMechanics::getTimeToGetDestFromV0y(float v0y) {
    const std::pair<float, float> times = solveQuadraticEquation(
        -gravitationalAccelerationEarth / 2.f,
        v0y,
        0.f
    );

    return times.first > 0.f
           ? times.first
           : times.second;
}

void ClassicalMechanics::timesShock(const std::vector<float> &v) {
    _timesShock = v;
}

void ClassicalMechanics::addShockFromPosition(float pos) {
    _timesShock.push_back(evalTimeFromPosX(pos));
}

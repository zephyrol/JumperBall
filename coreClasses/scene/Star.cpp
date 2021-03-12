/*
 * File: Star.cpp
 * Author: Morgenthaler S
 *
 * Created on 20 décembre 2019, 13:46
 */

#include "Star.h"

Star::Star(
    float radiusInside,
    float radiusOutside,
    float distance,
    float radius,
    const JBTypes::vec3f& rotationCenter):
    _radiusInside(radiusInside),
    _radiusOutside(radiusOutside),
    _distance(distance),
    _radius(radius),
    _rotationCenter(rotationCenter),
    _creationTime(JBTypesMethods::getTimePointMSNow()) {

}

float Star::radiusInside() const {
    return _radiusInside;
}

float Star::radiusOutside() const {
    return _radiusOutside;
}

float Star::distance() const {
    return _distance;
}

float Star::radius() const {
    return _radius;
}

float Star::getTimeSinceCreation() const {
    return JBTypesMethods::getTimeSecondsSinceTimePoint(_creationTime);
}

const JBTypes::vec3f& Star::rotationCenter() const {
    return _rotationCenter;
}

JBTypes::vec3f Star::lightDirection() const {
    const float angle = getTimeSinceCreation();
    const JBTypes::vec3f initialToStar = { 0.f, 0.f, -1.f };
    const JBTypes::vec3f rotationAxis = JBTypesMethods::normalize({ 0.5f, 1.f, 0.f });
    const JBTypes::vec3f toStar = JBTypesMethods::add(
        JBTypesMethods::scalarApplication(cosf(angle), initialToStar),
        JBTypesMethods::scalarApplication(sinf(angle), JBTypesMethods::cross(rotationAxis, initialToStar))
        );

    return JBTypesMethods::scalarApplication(-1.f, toStar);
}

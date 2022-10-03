/*
 * File: Star.cpp
 * Author: Morgenthaler S
 *
 * Created on 20 décembre 2019, 13:46
 */

#include "Star.h"

Star::Star(
    const Map &map,
    float radiusInside,
    float radiusOutside,
    float distance,
    float radius,
    const JBTypes::vec3f &initialDirection,
    const JBTypes::vec3f &rotationAxis,
    const JBTypes::vec3f &color,
    float radiansPerSecond
) :
    _radiusInside(radiusInside),
    _radiusOutside(radiusOutside),
    _distance(distance),
    _radius(radius),
    _initialDirection(initialDirection),
    _rotationAxis(rotationAxis),
    _rotationCenter(map.getCenterMap()),
    _envSize(map.getLargestSize()),
    _radiansPerSeconds(radiansPerSecond),
    _color(color),
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

const JBTypes::vec3f &Star::rotationCenter() const {
    return _rotationCenter;
}

float Star::envSize() const {
    return _envSize;
}

JBTypes::Quaternion Star::getRotation() const {
    const float angle = getTimeSinceCreation() * _radiansPerSeconds;
    return JBTypesMethods::createRotationQuaternion(_rotationAxis, angle);
}


JBTypes::vec3f Star::lightDirection() const {
    const float angle = getTimeSinceCreation() * _radiansPerSeconds;
    const JBTypes::vec3f toStar = JBTypesMethods::add(
        JBTypesMethods::scalarApplication(cosf(angle), _initialDirection),
        JBTypesMethods::scalarApplication(
            sinf(angle),
            JBTypesMethods::cross(_rotationAxis, _initialDirection))
    );

    return JBTypesMethods::scalarApplication(-2.f, toStar);
}

JBTypes::vec3f Star::position() const {
    const JBTypes::vec3f initialPosition = JBTypesMethods::scalarApplication(_distance, _initialDirection);
    return JBTypesMethods::rotateVector(initialPosition, getRotation());
}

Displayable::GlobalState Star::getGlobalState() const {
    return Displayable::GlobalState::United;
}

Displayable::StaticValues<JBTypes::vec3f> Star::getStaticVec3fValues() const {
    return {
        JBTypesMethods::scalarApplication(_distance, _initialDirection),
        _rotationCenter,
        _rotationAxis,
        _color
    };
}


Displayable::StaticValues<float> Star::getStaticFloatValues() const {
    return {
        _radiansPerSeconds
    };
}


Displayable::DynamicValues<float> Star::getDynamicFloats() const {
    return {
        {"timeSinceCreation", getTimeSinceCreation()}
    };
}

std::shared_ptr<Star> Star::createBlurStar(const Map &map) {
    const JBTypes::vec3f initialDirection = {0.f, 0.f, -1.f};
    const JBTypes::vec3f color = {0.f, 1.f, 1.f};
    return std::make_shared<Star>(
        map,
        0.3f,
        0.5f,
        50.f,
        5.f,
        initialDirection,
        JBTypesMethods::normalize({0.5f, 1.f, 0.f}),
        color,
        0.6f
    );
}

std::shared_ptr<Star> Star::createPurpleStar(const Map &map) {
    const JBTypes::vec3f initialDirection = {0.f, 0.f, -1.f};
    const JBTypes::vec3f color = {1.f, 0.f, 1.f};
    return std::make_shared<Star>(
        map,
        0.3f,
        0.5f,
        55.f,
        5.f,
        initialDirection,
        JBTypesMethods::normalize({0.5f, -1.f, 0.f}),
        color,
        1.1f
    );
}


/*
 * File: Star.cpp
 * Author: Morgenthaler S
 *
 * Created on 20 décembre 2019, 13:46
 */

#include "Star.h"

Star::Star(
    const Map &map,
    float distance,
    const JBTypes::vec3f &initialDirection,
    const JBTypes::vec3f &rotationAxis,
    const JBTypes::vec3f &color,
    float radiansPerSecond
) :
    _chronometer(map.getBall()->getCreationChronometer()),
    _distance(distance),
    _initialDirection(initialDirection),
    _rotationAxis(rotationAxis),
    _rotationCenter(map.getCenterMap()),
    _envSize(map.getLargestSize()),
    _radiansPerSeconds(radiansPerSecond),
    _color(color) {

}

const JBTypes::vec3f &Star::rotationCenter() const {
    return _rotationCenter;
}

float Star::envSize() const {
    return _envSize;
}

JBTypes::Quaternion Star::getRotation() const {
    const float angle = _chronometer->getTime() * _radiansPerSeconds;
    return JBTypesMethods::createRotationQuaternion(_rotationAxis, angle);
}

JBTypes::vec3f Star::lightDirection() const {
    const float angle = _chronometer->getTime() * _radiansPerSeconds;
    const JBTypes::vec3f toStar = JBTypesMethods::add(
        JBTypesMethods::scalarApplication(cosf(angle), _initialDirection),
        JBTypesMethods::scalarApplication(
            sinf(angle),
            JBTypesMethods::cross(_rotationAxis, _initialDirection)
        )
    );

    return JBTypesMethods::scalarApplication(-2.f, toStar);
}

JBTypes::vec3f Star::position() const {
    const JBTypes::vec3f initialPosition = JBTypesMethods::scalarApplication(_distance, _initialDirection);
    return JBTypesMethods::rotateVector(initialPosition, getRotation());
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

Displayable::DynamicNames Star::getDynamicFloatNames() const {
    return {"timeSinceCreation"};
}

Displayable::DynamicValues<float> Star::getDynamicFloatValues() const {
    return {_chronometer->getTime()};
}

std::shared_ptr<Star> Star::createBlurStar(const Map &map) {
    const JBTypes::vec3f initialDirection = {0.f, 0.f, -1.f};
    const JBTypes::vec3f color = {0.f, 1.f, 1.f};
    return std::make_shared<Star>(
        map,
        500.f,
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
        550.f,
        initialDirection,
        JBTypesMethods::normalize({0.5f, -1.f, 0.f}),
        color,
        1.1f
    );
}

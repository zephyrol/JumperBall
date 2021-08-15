/*
 * File: Item.cpp
 * Author: Morgenthaler S
 *
 * Created on 27 mars 2020, 19:40
 */

#include "Item.h"
#include <cmath>

Item::Item(
    const JBTypes::vec3ui& position,
    const JBTypes::Dir& direction
    ):
    _position(position),
    _direction(direction),
    _3Dposition(get3DPosition()),
    _gotten(false),
    _creationTime(JBTypesMethods::getTimePointMSNow()),
    _obtainingTime()
{
}

const JBTypes::vec3ui& Item::position() const {
    return _position;
}

const JBTypes::vec3f& Item::get3DPosition() const {
    return _3Dposition;
}

bool Item::isGotten() const {
    return _gotten;
}

float Item::getTimeSinceObtaining() const {
    return _gotten
           ? JBTypesMethods::getTimeSecondsSinceTimePoint(_obtainingTime)
           : 0;
}

float Item::getTimeSinceCreation() const {
    return JBTypesMethods::getTimeSecondsSinceTimePoint(_creationTime);
}

void Item::catchingTest (const JBTypes::vec3f& boundingSphereCenter, float boundingSphereRadius) {
    const float distance = JBTypesMethods::distance(_3Dposition, boundingSphereCenter);
    if (distance < boundingSphereRadius + itemBoundingSphereRadius) {
        _obtainingTime = JBTypesMethods::getTimePointMSNow();
        _gotten = true;
    }

}

SceneElement::StaticValues <float> Item::getStaticFloatValues() const {
    return { static_cast<float>(_direction) };
}

SceneElement::StaticValues <JBTypes::vec3f> Item::getStaticVec3fValues() const {
    constexpr float offset = 0.5;
    const JBTypes::vec3f position = {
        static_cast <float>(_position.at(0)) + offset,
        static_cast <float>(_position.at(1)) + offset,
        static_cast <float>(_position.at(2)) + offset
    };
    return { position };
}

SceneElement::DynamicValues <float> Item::getDynamicFloats() const {
    return {
        { "creationTime", getTimeSinceCreation()},
        { "obtainingTime", getTimeSinceObtaining()}
    };

}

SceneElement::GlobalState Item::getGlobalState() const {
    const float timeSinceObtaining = getTimeSinceObtaining();

    if (isGotten()) {
        constexpr float thresholdThirdStep = 1.5;
        constexpr float durationThirdStep = 0.2;
        return timeSinceObtaining < thresholdThirdStep + durationThirdStep
               ? SceneElement::GlobalState::Separate
               : SceneElement::GlobalState::Dead;
    }
    return SceneElement::GlobalState::United;
}

JBTypes::vec3f Item::compute3DPosition() const {
    constexpr float offsetPosition = 1.f;
    float x = static_cast <float>(_position.at(0) + 0.5f);
    float y = static_cast <float>(_position.at(1) + 0.5f);
    float z = static_cast <float>(_position.at(2) + 0.5f);

    switch (_direction)
    {
    case JBTypes::Dir::North:
        z -= offsetPosition;
        break;
    case JBTypes::Dir::South:
        z += offsetPosition;
        break;
    case JBTypes::Dir::East:
        x += offsetPosition;
        break;
    case JBTypes::Dir::West:
        x -= offsetPosition;
        break;
    case JBTypes::Dir::Up:
        y += offsetPosition;
        break;
    case JBTypes::Dir::Down:
        y -= offsetPosition;
        break;
    default:
        break;
    }

    return JBTypes::vec3f { x, y, z };
}

const JBTypes::Dir &Item::direction() const {
    return _direction;
}

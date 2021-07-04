/*
 * File: Item.cpp
 * Author: Morgenthaler S
 *
 * Created on 27 mars 2020, 19:40
 */

#include "Item.h"
#include <math.h>

Item::Item(
    const JBTypes::vec3ui& position,
    const JBTypes::Dir& direction,
    const CategoryOfItems& category
    ):
    _position(position),
    _direction(direction),
    _gotten(false),
    _creationTime(JBTypesMethods::getTimePointMSNow()),
    _obtainingTime(),
    _category(category) {
}

const Item::CategoryOfItems& Item::getCategory() const {
    return _category;
}

const JBTypes::vec3ui& Item::position() const {
    return _position;
}

const JBTypes::Dir& Item::direction() const {
    return _direction;
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

void Item::catchingTest (const JBTypes::vec3f& itemPosition,
                         const JBTypes::vec3f& entityPosition,
                         float radiusEntity) {
    const float distance = JBTypesMethods::distance(itemPosition, entityPosition);
    if (distance < radiusEntity + radiusBoundingSphere) {
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

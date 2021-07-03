/*
 * File: ItemState.cpp
 * Author: Morgenthaler S
 *
 * Created on 19 septembre 2020, 07h50
 */
#include "ItemState.h"

ItemState::ItemState(const Item& item):
    _item(item),
    _position(item.position()),
    _direction(static_cast <float>(item.direction())),
    _timeSinceCreation(item.getTimeSinceCreation()),
    _timeSinceObtaining(item.getTimeSinceObtaining()) {
}

ObjectState::GlobalState ItemState::update() {
    _timeSinceCreation = _item.getTimeSinceCreation();
    _timeSinceObtaining = _item.getTimeSinceObtaining();

    if (_item.isGotten()) {
        constexpr float thresholdThirdStep = 1.5;
        constexpr float durationThirdStep = 0.2;
        return _timeSinceObtaining < thresholdThirdStep + durationThirdStep
               ? ObjectState::GlobalState::Separate
               : ObjectState::GlobalState::Dead;
    }
    return ObjectState::GlobalState::United;
}

ObjectState::StaticValues <float> ItemState::getStaticFloatValues() const {
    return { _direction };
}

ObjectState::StaticValues <JBTypes::vec3f> ItemState::getStaticVec3fValues() const {
    constexpr float offset = 0.5;
    const JBTypes::vec3f position = {
        static_cast <float>(_position.at(0)) + offset,
        static_cast <float>(_position.at(1)) + offset,
        static_cast <float>(_position.at(2)) + offset
    };
    return { position };
}

ObjectState::DynamicValues <float> ItemState::getDynamicFloats() const {
    return {
        { "creationTime", _timeSinceCreation },
        { "obtainingTime", _timeSinceObtaining }
    };
}
/*
 * File: Item.cpp
 * Author: Morgenthaler S
 *
 * Created on 27 mars 2020, 19:40
 */

#include "Item.h"

Item::Item(const JBTypes::vec3ui &position, const JBTypes::Dir &direction, CstChronometer_sptr chronometer) :
    _chronometer(std::move(chronometer)),
    _position(position),
    _direction(direction),
    _3DPosition(compute3DPosition()),
    _gotten(false),
    _obtainingTime(0.f) {
}

const JBTypes::vec3ui &Item::position() const {
    return _position;
}

const JBTypes::vec3f &Item::get3DPosition() const {
    return _3DPosition;
}

bool Item::isGotten() const {
    return _gotten;
}

float Item::getTimeSinceObtaining() const {
    return _gotten
           ? _chronometer->getTime() - _obtainingTime
           : 0;
}

Displayable::StaticValues<unsigned char> Item::getStaticUnsignedByteValues() const {
    return {static_cast<unsigned char>(_direction)};
}

Displayable::StaticValues<JBTypes::vec3f> Item::getStaticVec3fValues() const {
    return {_3DPosition};
}

Displayable::DynamicNames Item::getDynamicFloatNames() const {
    return {"creationTime", "obtainingTime"};
}

Displayable::DynamicValues<float> Item::getDynamicFloatValues() const {
    return {_chronometer->getTime(), getTimeSinceObtaining()};
}

Displayable::GlobalState Item::getGlobalState() const {
    const float timeSinceObtaining = getTimeSinceObtaining();

    if (isGotten()) {
        constexpr float thresholdThirdStep = 1.5f;
        constexpr float durationThirdStep = 0.2f;
        return timeSinceObtaining < thresholdThirdStep + durationThirdStep
               ? Displayable::GlobalState::Separate
               : Displayable::GlobalState::Dead;
    }
    return Displayable::GlobalState::United;
}

JBTypes::vec3f Item::compute3DPosition() const {
    constexpr float offsetPosition = 0.755f;
    auto x = static_cast <float>(_position.at(0));
    auto y = static_cast <float>(_position.at(1));
    auto z = static_cast <float>(_position.at(2));

    switch (_direction) {
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

    return JBTypes::vec3f{x, y, z};
}

const JBTypes::Dir &Item::direction() const {
    return _direction;
}

void Item::setAsGotten() {
    _obtainingTime = _chronometer->getTime();
    _gotten = true;
}

bool Item::globalStateMayChange() const {
    return true;
}


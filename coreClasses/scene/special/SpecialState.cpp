/*
 * File: SpecialState.cpp
 * Author: Morgenthaler S
 *
 * Created on 14 novembre 2020, 18h09
 */
#include "SpecialState.h"

SpecialState::SpecialState(const Special& special, const Map::SpecialTypes& category):
    _special(special),
    _category(category),
    _color(special.getColor()),
    _creationTime(special.creationTime()),
    _direction(special.direction()),
    _position3D(special.position3D()),
    _isActivated(special.isActivated()) {
}

const JBTypes::Color& SpecialState::color() const {
    return _color;
}

const Map::SpecialTypes& SpecialState::category() const {
    return _category;
}

const JBTypes::Dir& SpecialState::direction() const {
    return _direction;
}

const JBTypes::vec3f& SpecialState::position3D() const {
    return _position3D;
}

const JBTypes::timePointMs& SpecialState::creationTime() const {
    return _creationTime;
}

bool SpecialState::isActivated() const {
    return _isActivated;
}

State::GlobalState SpecialState::update() {
    _isActivated = _special.isActivated();
    return GlobalState::United;
}

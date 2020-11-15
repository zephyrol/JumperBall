/*
 * File:   SpecialState.cpp
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
_position({0.f,0.f,0.f}), // TODO : feel it !
_transform({0.f,0.f,0.f,0.f,0.f,0.f,1.f,1.f,1.f}) // TODO: feel it !
{}

const JBTypes::Color& SpecialState::color() const {
   return _color; 
}

const Map::SpecialTypes& SpecialState::category() const {
   return _category; 
}

const JBTypes::vec3f& SpecialState::position() const {
   return _position; 
}

const std::array<float,9>& SpecialState::transform() const {
    return _transform;
}

void SpecialState::update() 
{}


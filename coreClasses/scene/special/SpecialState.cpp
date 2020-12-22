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
_direction(special.direction()),
_position(special.position())
{}

const JBTypes::Color& SpecialState::color() const {
   return _color; 
}

const Map::SpecialTypes& SpecialState::category() const {
   return _category; 
}

const JBTypes::Dir& SpecialState::direction() const {
   return _direction;
}

const JBTypes::vec3f& SpecialState::position() const {
   return _position; 
}

const JBTypes::timePointMs& SpecialState::creationTime() const {
   return _creationTime;
}

void SpecialState::update() {

}


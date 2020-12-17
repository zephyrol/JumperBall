/*
 * File:   SpecialState.h
 * Author: Morgenthaler S
 *
 * Created on 14 novembre 2020, 18h09
 */
#ifndef SPECIALSTATE_H
#define SPECIALSTATE_H
#include "Special.h"
#include <scene/Map.h>


class SpecialState
{
public:
    SpecialState(const Special& special, const Map::SpecialTypes& category);
    const JBTypes::Color& color() const;
    const Map::SpecialTypes& category() const;
    const JBTypes::Dir& direction() const;
    const JBTypes::vec3f& position() const;
    const std::array<float,9>& transform() const;
    void update();

private:
    const Special& _special;
    const Map::SpecialTypes& _category;
    const JBTypes::Color _color;
    const JBTypes::timePointMs& _creationTime;
    const JBTypes::Dir& _direction;
    JBTypes::vec3f _position;
    std::array<float,9> _transform;
};

#endif // SPECIALSTATE_H

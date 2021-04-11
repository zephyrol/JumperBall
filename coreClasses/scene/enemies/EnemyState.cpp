/*
 * File: EnemyState.cpp
 * Author: Morgenthaler S
 *
 * Created on 11 octobre 2020, 10h33
 */
#include "EnemyState.h"

EnemyState::EnemyState(const Enemy& enemy, const Map::EnemyTypes& category):
    _enemy(enemy),
    _category(category),
    _color(enemy.getColor()),
    _creationTime(enemy.creationTime()),
    _direction(enemy.direction()),
    _size(enemy.size()),
    _length(enemy.length()),
    _hasHit(enemy.hasHit()),
    _intersectionTime(enemy.intersectionTime()),
    _position(enemy.position()),
    _transform(enemy.transform()) {
}

const std::array <float, 9>& EnemyState::transform() const {
    return _transform;
}

State::GlobalState EnemyState::update() {
    _transform = _enemy.transform();
    return State::GlobalState::Separate;
}

State::DynamicValues <JBTypes::vec3f> EnemyState::getDynamicVec3fs() const {
    return {
        { "translation", { _transform.at(0), _transform.at(1), _transform.at(2) }},
        { "scale", { _transform.at(6), _transform.at(7), _transform.at(8) }}
    };
}

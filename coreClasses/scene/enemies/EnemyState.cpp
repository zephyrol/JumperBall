/*
 * File: EnemyState.cpp
 * Author: Morgenthaler S
 *
 * Created on 11 octobre 2020, 10h33
 */
#include "EnemyState.h"

EnemyState::EnemyState(const Enemy& enemy):
    _enemy(enemy),
    _transform(enemy.transform()) {
}

const std::array <float, 9>& EnemyState::transform() const {
    return _transform;
}

ObjectState::GlobalState EnemyState::update() {
    _transform = _enemy.transform();
    return ObjectState::GlobalState::Separate;
}

ObjectState::DynamicValues <JBTypes::vec3f> EnemyState::getDynamicVec3fs() const {
    return {
        { "translation", { _transform.at(0), _transform.at(1), _transform.at(2) }},
        { "scale", { _transform.at(6), _transform.at(7), _transform.at(8) }}
    };
}

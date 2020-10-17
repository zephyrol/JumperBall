/*
 * File:   EnemyState.cpp
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
_hasHit(enemy.hasHit()),
_intersectionTime(enemy.intersectionTime()),
_position(enemy.position()),
_transform{0.f,0.f,0.f,0.f,0.f,0.f,1.f,1.f,1.f}
{
}

const Enemy::Color& EnemyState::color() const {
   return _color; 
}

const Map::EnemyTypes& EnemyState::category() const {
   return _category; 
}

const JBTypes::vec3f& EnemyState::position() const {
   return _position; 
}

const std::array<float,9>& EnemyState::transform() const {
    return _transform;
}

void EnemyState::update() {
    _hasHit = _enemy.hasHit();
    _intersectionTime = _enemy.intersectionTime();
    _position = _enemy.position();
    _transform = _enemy.transform();
}


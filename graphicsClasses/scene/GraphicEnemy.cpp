/*
 * File:   GraphicEnemy.cpp
 * Author: Morgenthaler S
 *
 * Created on 11 octobre 2020, 10h33
 */
#include "GraphicEnemy.h"

GraphicEnemy::GraphicEnemy(const Enemy& enemy, const Map::EnemyTypes& category):
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

const Enemy::Color& GraphicEnemy::color() const {
   return _color; 
}

const Map::EnemyTypes& GraphicEnemy::category() const {
   return _category; 
}

const JBTypes::vec3f& GraphicEnemy::position() const {
   return _position; 
}

const std::array<float,9>& GraphicEnemy::transform() const {
    return _transform;
}

void GraphicEnemy::update() {
    _hasHit = _enemy.hasHit();
    _intersectionTime = _enemy.intersectionTime();
    _position = _enemy.position();
    _transform = _enemy.transform();
}


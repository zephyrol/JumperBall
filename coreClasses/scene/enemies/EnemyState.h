/*
 * File: EnemyState.h
 * Author: Morgenthaler S
 *
 * Created on 11 octobre 2020, 10h33
 */
#ifndef ENEMYSTATE_H
#define ENEMYSTATE_H
#include <scene/enemies/Enemy.h>
#include <scene/Map.h>
#include <scene/State.h>


class EnemyState:State {
public:
EnemyState(const Enemy& enemy, const Map::EnemyTypes& category);
const JBTypes::Color& color() const;
const Map::EnemyTypes& category() const;
const JBTypes::vec3f& position() const;
const JBTypes::Direction& direction() const;
const float& size() const;
const size_t& length() const;
const std::array <float, 9>& transform() const;
virtual void update() override;

private:
const Enemy& _enemy;
const Map::EnemyTypes& _category;
const JBTypes::Color _color;
const JBTypes::timePointMs& _creationTime;
const JBTypes::Direction& _direction;
const float _size;
const size_t _length;
bool _hasHit;
JBTypes::timePointMs _intersectionTime;
JBTypes::vec3f _position;
std::array <float, 9> _transform;
};

#endif // ENEMYSTATE_H

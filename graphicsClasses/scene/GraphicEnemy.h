/*
 * File:   GraphicEnemy.h
 * Author: Morgenthaler S
 *
 * Created on 11 octobre 2020, 10h33
 */
#ifndef GRAPHICENEMY_H
#define GRAPHICENEMY_H
#include <scene/enemies/Enemy.h>
#include <scene/Map.h>


class GraphicEnemy
{
public:
    GraphicEnemy(const Enemy& enemy, const Map::EnemyTypes& category);
    const Enemy::Color& color() const;
    const Map::EnemyTypes& category() const;
    const JBTypes::vec3f& position() const;
    const std::array<float,9>& transform() const;
    void update();

private:
    const Enemy& _enemy;
    const Map::EnemyTypes& _category;
    const Enemy::Color _color;
    const JBTypes::timePointMs& _creationTime;
    bool _hasHit;
    JBTypes::timePointMs _intersectionTime;
    JBTypes::vec3f _position;
    std::array<float,9> _transform;

};

#endif // GRAPHICENEMY_H

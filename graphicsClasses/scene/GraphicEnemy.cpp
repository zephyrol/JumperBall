/*
 * File:   GraphicEnemy.cpp
 * Author: Morgenthaler S
 *
 * Created on 11 octobre 2020, 10h33
 */
#include "GraphicEnemy.h"

GraphicEnemy::GraphicEnemy(const Enemy& enemy) :
_enemy(enemy),
_creationTime(enemy.creationTime()),
_hasHit(enemy.hasHit()),
_intersectionTime(enemy.intersectionTime()),
_position(enemy.position())
{
}

void GraphicEnemy::update() {
    _hasHit = _enemy.hasHit();
    _intersectionTime = _enemy.intersectionTime();
    _position = _enemy.position();
}


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


class EnemyState:public State {
public:
EnemyState(const Enemy& enemy);
const std::array <float, 9>& transform() const;
virtual State::GlobalState update() override;

virtual State::DynamicValues <JBTypes::vec3f> getDynamicVec3fs() const override;

private:
const Enemy& _enemy;
std::array <float, 9> _transform;
};

#endif // ENEMYSTATE_H

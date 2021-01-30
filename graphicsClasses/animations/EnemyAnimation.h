#ifndef ENEMYANIMATION_H
#define ENEMYANIMATION_H
#include <scene/enemies/EnemyState.h>
#include "Utility.h"
#include "Animation.h"

/*
 * File: EnemyAnimation.h
 * Author: Morgenthaler S
 *
 * Created on 15 aout 2020, 11h00
 */

class EnemyAnimation:public Animation {
public:

// --CONSTRUCTORS & DESTRUCTORS--//
EnemyAnimation(const EnemyState& enemy);
virtual ~EnemyAnimation() = default;

// ----------METHODS------------//
void updateTrans() override;

private:
// --------ATTRIBUTES-----------//
const EnemyState& _enemy;
};


#endif // ENEMYANIMATION_H

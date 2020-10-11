/*
 * File:   GraphicEnemy.h
 * Author: Morgenthaler S
 *
 * Created on 11 octobre 2020, 10h33
 */
#ifndef GRAPHICENEMY_H
#define GRAPHICENEMY_H
#include <enemies/Enemy.h>


class GraphicEnemy
{
public:
    GraphicEnemy(const Enemy& enemy);
    void update();

private:
    const Enemy& _enemy;
};

#endif // GRAPHICENEMY_H

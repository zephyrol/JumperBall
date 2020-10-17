/*
 * File:   genEnemy.cpp
 * Author: Morgenthaler S
 *
 * Created on 22 aout 2020, 18h15
 */
#include "../scene/mesh/MeshGenerator.h"

vecMeshComponent_sptr MeshGenerator::genEnemy(const GraphicEnemy& enemy) {

    vecMeshComponent_sptr components;
    if (enemy.category() == Map::EnemyTypes::Laser)
    {
        const Enemy::Color color = enemy.color();
        switch (color)
        {
        case Enemy::Color::Blue:
            if (commonShapes.find("blueLaser") == commonShapes.end())
                commonShapes["blueLaser"] = std::make_shared<Cylinder>(
                    glm::vec3(0.f, 0.f, 1.f));
            break;
        case Enemy::Color::Red:
            if (commonShapes.find("redLaser") == commonShapes.end())
                commonShapes["redLaser"] = std::make_shared<Cylinder>(
                    glm::vec3(1.f, 0.f, 0.f));
            break;
        case Enemy::Color::Green:
            if (commonShapes.find("greenLaser") == commonShapes.end())
                commonShapes["greenLaser"] = std::make_shared<Cylinder>(
                    glm::vec3(0.f, 1.f, 0.f));
            break;
        case Enemy::Color::Yellow:
            if (commonShapes.find("yellowLaser") == commonShapes.end())
                commonShapes["yellowLaser"] = std::make_shared<Cylinder>(
                    glm::vec3(1.f, 1.f, 0.f));
            break;
        case Enemy::Color::None:
            break;
        default:
            break;
        }
        const JBTypes::vec3f& position = enemy.position();
        const glm::vec3 glmPosition { position.x, position.y, position.z };
    }
    else if (enemy.category() == Map::EnemyTypes::ThornBall)
    {
    }
else if (enemy.category() == Map::EnemyTypes::DarkBall)
{
}

    return components;
}

/*
 * File:   genEnemy.cpp
 * Author: Morgenthaler S
 *
 * Created on 22 aout 2020, 18h15
 */
#include "../scene/mesh/MeshGenerator.h"

vecMeshComponent_sptr MeshGenerator::genEnemy(const GraphicEnemy& enemy,
                                              const Map::EnemyTypes& category) {
    
    vecMeshComponent_sptr components;
    if ( category == Map::EnemyTypes::Laser) {
        const Enemy::Color color = enemy.color();
        switch (color) {
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
            default: break;
        }

    } else if ( category == Map::EnemyTypes::ThornBall) {

    } else if ( category == Map::EnemyTypes::DarkBall) {

    }

    return components;
}

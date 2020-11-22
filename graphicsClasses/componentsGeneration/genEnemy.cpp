/*
 * File:   genEnemy.cpp
 * Author: Morgenthaler S
 *
 * Created on 22 aout 2020, 18h15
 */
#include "scene/mesh/MeshGenerator.h"

vecMeshComponent_sptr MeshGenerator::genEnemy(const EnemyState& enemy) {

    vecMeshComponent_sptr components;
    if (enemy.category() == Map::EnemyTypes::Laser)
    {
        const JBTypes::Color color = enemy.color();
        std::string laserName;
        switch (color)
        {
        case JBTypes::Color::Blue:
            laserName = "blueLaser";
            if (commonShapes.find(laserName) == commonShapes.end())
                commonShapes[laserName] = std::make_shared<Cylinder>(
                    glm::vec3(0.f, 0.f, 1.f));
            break;
        case JBTypes::Color::Red:
            laserName = "redLaser";
            if (commonShapes.find(laserName) == commonShapes.end())
                commonShapes[laserName] = std::make_shared<Cylinder>(
                    glm::vec3(1.f, 0.f, 0.f));
            break;
        case JBTypes::Color::Green:
            laserName = "greenLaser";
            if (commonShapes.find(laserName) == commonShapes.end())
                commonShapes[laserName] = std::make_shared<Cylinder>(
                    glm::vec3(0.f, 1.f, 0.f));
            break;
        case JBTypes::Color::Yellow:
            laserName = "yellowLaser";
            if (commonShapes.find(laserName) == commonShapes.end())
                commonShapes[laserName] = std::make_shared<Cylinder>(
                    glm::vec3(1.f, 1.f, 0.f));
            break;
        case JBTypes::Color::None:
            break;
        default:
            break;
        }
        const JBTypes::vec3f& position = enemy.position();
        const glm::vec3 glmPosition { position.x, position.y, position.z };
        const glm::vec3 scale { 0.1f, enemy.size(), 0.1f };

        const glm::mat4 translationMatrix = glm::translate(glmPosition);
        const glm::mat4 scaleMatrix = glm::scale(scale);
        const glm::mat4 tranformLocal = translationMatrix * scaleMatrix;

        const MeshComponent_sptr componentCylinder =
            std::make_shared<MeshComponent>(
                std::make_shared<Cylinder>(
                    *commonShapes.at(laserName),
                    tranformLocal),
                std::make_shared<EnemyAnimation>(enemy));
        components.push_back(componentCylinder);
    }
    else if (enemy.category() == Map::EnemyTypes::ThornBall ||
             enemy.category() == Map::EnemyTypes::DarkBall) {
        const float radius = enemy.category() == Map::EnemyTypes::ThornBall 
            ? ThornBall::thornBallRadius
            : DarkBall::darkBallRadius;

        const glm::mat4 scaleMatrix = glm::scale(glm::vec3(radius));
        const Sphere sphere;
        const MeshComponent_sptr component = std::make_shared<MeshComponent>
                (std::make_shared<Sphere>(sphere,scaleMatrix),
                 std::make_shared<EnemyAnimation>(enemy));
        components.push_back(component);
    }

    return components;
}

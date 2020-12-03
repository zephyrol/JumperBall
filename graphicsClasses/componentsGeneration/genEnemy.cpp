/*
 * File:   genEnemy.cpp
 * Author: Morgenthaler S
 *
 * Created on 22 aout 2020, 18h15
 */
#include "scene/mesh/MeshGenerator.h"

vecMeshComponent_sptr MeshGenerator::genEnemy(const EnemyState &enemy)
{

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
        const JBTypes::vec3f &position = enemy.position();
        const glm::vec3 glmPosition{position.x, position.y, position.z};
        const glm::vec3 scale{0.1f, enemy.size(), 0.1f};

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
             enemy.category() == Map::EnemyTypes::DarkBall)
    {

        const JBTypes::Dir &currentDir = enemy.direction();
        const glm::mat4 rotationLocal =
            Utility::rotationUpToDir(currentDir);

        const JBTypes::vec3f &posWorld = enemy.position();

        const glm::mat4 scaleLocal = glm::scale(glm::vec3(enemy.size()));

        const glm::mat4 translationLocal =
            glm::translate(glm::vec3( posWorld.x, posWorld.y, posWorld.z));

        const glm::mat4 modelTransf = 
            translationLocal * rotationLocal * scaleLocal;
        const glm::mat4 normalsTransf = rotationLocal;

        const std::string sphereName =
            enemy.category() == Map::EnemyTypes::ThornBall
            ? "thornSphere"
            : "darkSphere";
        if (commonShapes.find(sphereName) == commonShapes.end()) {
            const glm::vec3 sphereColor =
                enemy.category() == Map::EnemyTypes::ThornBall
                ? glm::vec3(114.f/255.f,47.f/255.f,55.f/255.f)
                : glm::vec3(0.f,0.f,0.5f);
            commonShapes[sphereName] = std::make_shared<Sphere>(sphereColor);
        }
        const MeshComponent_sptr component =
            std::make_shared<MeshComponent>(std::make_shared<Sphere>(
                                                *commonShapes.at(sphereName), 
                                                modelTransf, 
                                                normalsTransf),
                                            std::make_shared<EnemyAnimation>(
                                                enemy)
            );
        components.push_back(component);
    }

    return components;
}

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

        constexpr float laserIntensity = 1.5f;
        // Lasers create the bloom effect, but we would like that
        // all laser colors have the same brightness;
        const float redConeSensibilityCoeff = 0.3;
        const float greenConeSensibilityCoeff = 0.59;
        const float blueConeSensibilityCoeff = 0.11;

        const JBTypes::Color color = enemy.color();
        std::string laserName;
        switch (color)
        {
        case JBTypes::Color::Blue:
            laserName = "blueLaser";
            if (commonShapes.find(laserName) == commonShapes.end())
                commonShapes[laserName] = std::make_shared<Cylinder>(
                    laserIntensity * glm::vec3(0.f,
                                               1.f/greenConeSensibilityCoeff,
                                               1.f/blueConeSensibilityCoeff));
            break;
        case JBTypes::Color::Red:
            laserName = "redLaser";
            if (commonShapes.find(laserName) == commonShapes.end())
                commonShapes[laserName] = std::make_shared<Cylinder>(
                    laserIntensity * 
                    glm::vec3(1.f/redConeSensibilityCoeff, 0.f, 0.f));
            break;
        case JBTypes::Color::Green:
            laserName = "greenLaser";
            if (commonShapes.find(laserName) == commonShapes.end())
                commonShapes[laserName] = std::make_shared<Cylinder>(
                    laserIntensity * 
                    glm::vec3(0.f, 1.f/greenConeSensibilityCoeff, 0.f));
            break;
        case JBTypes::Color::Yellow:
            laserName = "yellowLaser";
            if (commonShapes.find(laserName) == commonShapes.end())
                commonShapes[laserName] = std::make_shared<Cylinder>(
                    laserIntensity * glm::vec3(1.f/redConeSensibilityCoeff, 
                                               1.f/greenConeSensibilityCoeff,
                                               0.f));
            break;
        case JBTypes::Color::None:
            break;
        default:
            break;
        }

        constexpr float offsetLaserSide = 0.15f;
        const JBTypes::vec3f &position = enemy.position();
        const glm::vec3 glmPosition{position.x, position.y, position.z};
        const glm::vec3 scale
            { enemy.size(), static_cast<float>(enemy.length()), enemy.size() };

        const JBTypes::Dir &currentDir = enemy.direction();
        const glm::mat4 rotationMatrix =
            Utility::rotationUpToDir(currentDir);

        const glm::mat4 translationMatrix = glm::translate(glmPosition);
        const glm::mat4 scaleMatrix = glm::scale(scale);

        const glm::mat4 translationOffsetLaserA = 
            glm::translate(glm::vec3( -offsetLaserSide, 0.f, -offsetLaserSide));
        const glm::mat4 translationOffsetLaserB = 
            glm::translate(glm::vec3( offsetLaserSide, 0.f, -offsetLaserSide));
        const glm::mat4 translationOffsetLaserC = 
            glm::translate(glm::vec3( offsetLaserSide, 0.f, offsetLaserSide));
        const glm::mat4 translationOffsetLaserD = 
            glm::translate(glm::vec3( -offsetLaserSide, 0.f, offsetLaserSide));
        
        const glm::mat4 modelTransfLaserOne = translationMatrix * rotationMatrix 
            * translationOffsetLaserA * scaleMatrix;
        const glm::mat4 modelTransfLaserTwo = translationMatrix * rotationMatrix 
            * translationOffsetLaserB * scaleMatrix;
        const glm::mat4 modelTransfLaserThree = translationMatrix * rotationMatrix 
            * translationOffsetLaserC * scaleMatrix;
        const glm::mat4 modelTransfLaserFour = translationMatrix * rotationMatrix 
            * translationOffsetLaserD * scaleMatrix;
        
        const std::array<glm::mat4, 4> lasersTransf {
            modelTransfLaserOne,
            modelTransfLaserTwo,
            modelTransfLaserThree,
            modelTransfLaserFour
        };

        const glm::mat4 normalsTransf = rotationMatrix;

        for ( const glm::mat4& laserTransf: lasersTransf) {
            const MeshComponent_sptr componentCylinder =
                std::make_shared<MeshComponent>(
                    std::make_shared<Cylinder>(
                        *commonShapes.at(laserName),
                        laserTransf,
                        normalsTransf),
                    std::make_shared<EnemyAnimation>(enemy));
            components.push_back(componentCylinder);
        }
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

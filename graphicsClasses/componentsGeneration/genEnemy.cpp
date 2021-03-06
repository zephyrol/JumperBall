/*
 * File: genEnemy.cpp
 * Author: Morgenthaler S
 *
 * Created on 22 aout 2020, 18h15
 */
#include "MeshGenerator.h"

Mesh_sptr MeshGenerator::genEnemy (
    const std::shared_ptr<const Enemy>& enemy,
    const Map::EnemyTypes& category
    ) {

    vecCstGeometricShape_sptr geometricShapes;
    if (category == Map::EnemyTypes::Laser) {

        const JBTypes::Color color = enemy->getColor();
        std::string laserName;

        const auto createLaserShape =
            [] (const JBTypes::Color& color,
                const glm::mat4& model,
                const glm::mat4 normals)->CstGeometricShape_sptr {

                constexpr float laserIntensity = 1.5f;
                // Lasers create the bloom effect, but we would like that
                // all laser colors have the same brightness;
                constexpr float redConeSensibilityCoeff = 0.3f;
                constexpr float greenConeSensibilityCoeff = 0.59f;
                constexpr float blueConeSensibilityCoeff = 0.11f;

                glm::vec3 laserColor;
                switch (color) {
                case JBTypes::Color::Blue:
                    laserColor = laserIntensity *
                                 glm::vec3(0.f, 1.f / greenConeSensibilityCoeff,
                                           1.f / blueConeSensibilityCoeff);
                    break;
                case JBTypes::Color::Red:
                    laserColor = laserIntensity * glm::vec3(1.f / redConeSensibilityCoeff, 0.f, 0.f);
                    break;
                case JBTypes::Color::Green:
                    laserColor = laserIntensity * glm::vec3(0.f, 1.f / greenConeSensibilityCoeff, 0.f);
                    break;
                case JBTypes::Color::Yellow:
                    laserColor = laserIntensity * glm::vec3(1.f / redConeSensibilityCoeff,
                                                            1.f / greenConeSensibilityCoeff,
                                                            0.f);
                    break;
                case JBTypes::Color::None:
                    break;
                default:
                    break;
                }
                return std::make_shared <Cylinder>(laserColor, 60, model, normals);
            };

        constexpr float offsetLaserSide = 0.15f;
        const JBTypes::vec3f& position = enemy->position();
        const glm::vec3 glmPosition { position.x, position.y, position.z };
        const glm::vec3 scale { enemy->size(), static_cast <float>(enemy->length()), enemy->size() };

        const JBTypes::Dir& currentDir = enemy->direction();
        const glm::mat4 rotationMatrix = Utility::rotationUpToDir(currentDir);

        const glm::mat4 translationMatrix = glm::translate(glmPosition);
        const glm::mat4 scaleMatrix = glm::scale(scale);

        const glm::mat4 translationOffsetLaserA =
            glm::translate(glm::vec3(-offsetLaserSide, 0.f, -offsetLaserSide));
        const glm::mat4 translationOffsetLaserB =
            glm::translate(glm::vec3(offsetLaserSide, 0.f, -offsetLaserSide));
        const glm::mat4 translationOffsetLaserC =
            glm::translate(glm::vec3(offsetLaserSide, 0.f, offsetLaserSide));
        const glm::mat4 translationOffsetLaserD =
            glm::translate(glm::vec3(-offsetLaserSide, 0.f, offsetLaserSide));

        const glm::mat4 translationRotation = translationMatrix * rotationMatrix;

        const glm::mat4 modelTransfLaserOne = translationRotation * translationOffsetLaserA * scaleMatrix;
        const glm::mat4 modelTransfLaserTwo = translationRotation * translationOffsetLaserB * scaleMatrix;
        const glm::mat4 modelTransfLaserThree = translationRotation * translationOffsetLaserC * scaleMatrix;
        const glm::mat4 modelTransfLaserFour = translationRotation * translationOffsetLaserD * scaleMatrix;

        const std::array <glm::mat4, 4> lasersTransf {
            modelTransfLaserOne,
            modelTransfLaserTwo,
            modelTransfLaserThree,
            modelTransfLaserFour
        };

        const glm::mat4 normalsTransf = rotationMatrix;

        for (const glm::mat4& laserTransf : lasersTransf) {
            geometricShapes.push_back(createLaserShape(color, laserTransf, normalsTransf));
        }
    } else if (category == Map::EnemyTypes::ThornBall || category == Map::EnemyTypes::DarkBall) {

        const JBTypes::Dir& currentDir = enemy->direction();
        const glm::mat4 rotationLocal = Utility::rotationUpToDir(currentDir);

        const JBTypes::vec3f& posWorld = enemy->position();

        const glm::mat4 scaleLocal = glm::scale(glm::vec3(enemy->size()));

        const glm::mat4 translationLocal = glm::translate(glm::vec3(posWorld.x, posWorld.y, posWorld.z));

        const glm::mat4 modelTransf = translationLocal * rotationLocal * scaleLocal;
        const glm::mat4 normalsTransf = rotationLocal;

        const glm::vec3 sphereColor = category == Map::EnemyTypes::ThornBall
                                      ? glm::vec3(114.f / 255.f, 47.f / 255.f, 55.f / 255.f)
                                      : glm::vec3(0.f, 0.f, 0.5f);
        geometricShapes.push_back(std::make_shared <const Sphere>(
                                      sphereColor,
                                      modelTransf,
                                      normalsTransf));
    }

    return std::make_shared <Mesh>(enemy, std::move(geometricShapes));
}

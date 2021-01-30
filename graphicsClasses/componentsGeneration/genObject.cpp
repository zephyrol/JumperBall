/*
 * File: genObject.cpp
 * Author: Morgenthaler S
 *
 * Created on 22 aout 2020, 18h15
 */
#include "scene/mesh/MeshGenerator.h"
#include "animations/ObjectAnimation.h"

vecMeshComponent_sptr MeshGenerator::genObject (
    const ObjectState& obj,
    const glm::vec3& position,
    const JBTypes::Dir& dir) {
    vecMeshComponent_sptr components;
    switch (obj.getCategory()) {
    case Object::CategoryOfObjects::Key: {

        constexpr size_t nbGeometriesToCreateAKey = 4;

        if (commonShapes.find("goldenSphere") == commonShapes.end()) {
            commonShapes["goldenSphere"] = std::make_shared <Sphere>
                                               (glm::vec3(1.f, 215.f / 255.f, 0.f));
        }
        if (commonShapes.find("goldenCube") == commonShapes.end()) {
            commonShapes["goldenCube"] = std::make_shared <Cube>
                                             (glm::vec3(1.f, 215.f / 255.f, 0.f));
        }

        const std::array <glm::vec3, nbGeometriesToCreateAKey> scales
        { glm::vec3(0.09f, 0.075f, 0.050f), glm::vec3(0.05f, 0.3f, 0.05f),
          glm::vec3(0.1f, 0.05f, 0.05f), glm::vec3(0.1f, 0.05f, 0.05f) };

        const std::array <glm::vec3, nbGeometriesToCreateAKey>
        translations
        { glm::vec3(0.f, 0.175f, 0.f),
          glm::vec3(-0.025f, -0.175f, -0.025f),
          glm::vec3(-0.025f, -0.075f, -0.025f),
          glm::vec3(-0.025f, -0.175f, -0.025f)
        };

        for (unsigned int i = 0; i < nbGeometriesToCreateAKey; ++i) {
            const glm::mat4 scaleMatrix = glm::scale(scales.at(i));
            const glm::mat4 translationMatrix =
                glm::translate(translations.at(i));
            const glm::mat4 tranform = translationMatrix * scaleMatrix;
            if (i == 0) {
                MeshComponent_sptr componentSphere =
                    std::make_shared <MeshComponent>
                        (std::make_shared <Sphere>(
                            *commonShapes.at("goldenSphere"),
                            tranform),
                        std::make_shared <ObjectAnimation>(obj, position, dir));
                components.push_back(componentSphere);
            } else {
                MeshComponent_sptr componentCube =
                    std::make_shared <MeshComponent>
                        (std::make_shared <Cube>(*commonShapes.at("goldenCube"),
                                                 tranform),
                        std::make_shared <ObjectAnimation>(obj, position, dir));
                components.push_back(componentCube);
            }
        }
        break;
    }
    case Object::CategoryOfObjects::Coin: {

        if (commonShapes.find("coinCylinder") == commonShapes.end()) {
            commonShapes["coinCylinder"] = std::make_shared <Cylinder>
                                               (glm::vec3(1.f, 215.f / 255.f, 0.f),
                                               glm::vec3(150.f / 255.f, 75.f / 255.f, 0.f));
        }

        const glm::vec3 scale { 0.3f, 0.05f, 0.3f };
        const glm::vec3 translation { 0.f, 0.f, -0.025f };

        const glm::mat4 scaleMatrix = glm::scale(scale);
        const glm::mat4 translationMatrix = glm::translate(translation);
        const glm::mat4 rotationMatrix =
            glm::rotate(static_cast <float>(M_PI / 2.),
                        glm::vec3(1.f, 0.f, 0.f));

        const glm::mat4 tranformLocal =
            translationMatrix * rotationMatrix * scaleMatrix;
        const glm::mat4 tranformNormals = rotationMatrix;

        const MeshComponent_sptr componentCube =
            std::make_shared <MeshComponent>(
                std::make_shared <Cylinder>(
                    *commonShapes.at("coinCylinder"),
                    tranformLocal,
                    tranformNormals),
                std::make_shared <ObjectAnimation>(obj, position, dir));
        components.push_back(componentCube);
        break;
    }
    case Object::CategoryOfObjects::Clock: {
        break;
    }
    default: break;
    }
    return components;
}

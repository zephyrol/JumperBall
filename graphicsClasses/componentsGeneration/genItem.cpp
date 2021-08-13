/*
 * File: genItem.cpp
 * Author: Morgenthaler S
 *
 * Created on 22 aout 2020, 18h15
 */
#include "MeshGenerator.h"

/*Mesh_sptr MeshGenerator::genItem (const std::shared_ptr<const Item>& item) {

    vecCstGeometricShape_sptr geometricShapes;
    switch (item->getCategory()) {
    case Item::CategoryOfItems::Key:
    {

        constexpr size_t nbGeometriesToCreateAKey = 4;
        const std::array <glm::vec3, nbGeometriesToCreateAKey> scales { glm::vec3(0.09f, 0.075f, 0.050f),
                                                                        glm::vec3(0.05f, 0.3f, 0.05f),
                                                                        glm::vec3(0.1f, 0.05f, 0.05f),
                                                                        glm::vec3(0.1f, 0.05f, 0.05f) };
        const std::array <glm::vec3, nbGeometriesToCreateAKey>
        translations { glm::vec3(0.f, 0.175f, 0.f),
                       glm::vec3(-0.025f, -0.175f, -0.025f),
                       glm::vec3(-0.025f, -0.075f, -0.025f),
                       glm::vec3(-0.025f, -0.175f, -0.025f) };

        for (unsigned int i = 0; i < nbGeometriesToCreateAKey; ++i) {
            const glm::mat4 scaleMatrix = glm::scale(scales.at(i));
            const glm::mat4 translationMatrix =
                glm::translate(translations.at(i));
            const glm::mat4 transform = translationMatrix * scaleMatrix;
            if (i == 0) {
                geometricShapes.push_back(std::make_shared <const Sphere>(
                                              glm::vec3(1.f, 215.f / 255.f, 0.f),
                                              transform
                                              ));
            } else {
                geometricShapes.push_back(std::make_shared <const Cube>(
                                              glm::vec3(1.f, 215.f / 255.f, 0.f),
                                              transform
                                              ));
            }
        }
        break;
    }
    case Item::CategoryOfItems::Coin:
    {
        const glm::vec3 scale { 0.3f, 0.05f, 0.3f };
        const glm::vec3 translation { 0.f, 0.f, -0.025f };

        const glm::mat4 scaleMatrix = glm::scale(scale);
        const glm::mat4 translationMatrix = glm::translate(translation);
        const glm::mat4 rotationMatrix =
            glm::rotate(static_cast <float>(M_PI / 2.),
                        glm::vec3(1.f, 0.f, 0.f));

        const glm::mat4 transformLocal =
            translationMatrix * rotationMatrix * scaleMatrix;
        const glm::mat4 transformNormals = rotationMatrix;

        geometricShapes.push_back(
            std::make_shared <const Cylinder>(glm::vec3(1.f, 215.f / 255.f, 0.f),
                                              glm::vec3(150.f / 255.f, 75.f / 255.f, 0.f),
                                              60,
                                              transformLocal,
                                              transformNormals)
            );
        break;
    }
    case Item::CategoryOfItems::Clock:
        break;
    default:
        break;
    }
    return std::make_shared <Mesh>( item, std::move(geometricShapes)); 
}*/

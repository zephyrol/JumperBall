/*
 * File: genJumpers.cpp
 * Author: Morgenthaler S
 *
 * Created on 22 aout 2020, 18h15
 */

#include "MeshGenerator.h"


vecCstGeometricShape_sptr MeshGenerator::genJumpers (const Block& block,
                                                     const Map::BlockTypes& type,
                                                     const glm::vec3& posWorld) {
    if (type != Map::BlockTypes::Jump) {
        return {};
    }
    vecCstGeometricShape_sptr geometricShapes {};

    for (size_t i = 0; i < block.faceInfo().size(); i++) {
        const bool jumperPresent = block.faceInfo().at(i);

        if (jumperPresent) {
            constexpr float sizeBlock = 1.f;
            constexpr float offset = sizeBlock / 2.f;

            const JBTypes::Dir currentDir = JBTypesMethods::integerAsDirection(static_cast <unsigned int>(i));

            const JBTypes::vec3f vecDir = JBTypesMethods::directionAsVector(currentDir);

            const glm::mat4 rotationLocal = Utility::rotationUpToDir(currentDir);

            const glm::mat4 scaleLocal = glm::scale(glm::vec3(0.7f, 0.05f, 0.7f));

            const glm::mat4 translationLocal =
                glm::translate(glm::vec3(
                                   posWorld.x + offset + vecDir.x * offset,
                                   posWorld.y + offset + vecDir.y * offset,
                                   posWorld.z + offset + vecDir.z * offset));

            const glm::mat4 modelTranf = translationLocal * rotationLocal * scaleLocal;
            const glm::mat4 normalsTrans = rotationLocal;

            geometricShapes.push_back(std::make_shared <const Cylinder>(
                                          glm::vec3(1.f, 1.f, 0.f),
                                          glm::vec3(1.f, 150.f / 255.f, 0.f),
                                          60,
                                          modelTranf,
                                          normalsTrans
                                          ));
        }
    }
    return geometricShapes;
}

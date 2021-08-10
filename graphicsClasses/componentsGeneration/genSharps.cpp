/*
 * File: genSharps.cpp
 * Author: Morgenthaler S
 *
 * Created on 22 aout 2020, 18h15
 */

#include "MeshGenerator.h"

vecCstGeometricShape_sptr MeshGenerator::genSharps (
    const Block& block,
    const Map::BlockTypes& type,
    const glm::vec3& posWorld
) {
    if (type != Map::BlockTypes::Sharp) {
        return {};
    }

    vecCstGeometricShape_sptr geometricShapes {};
    const std::array <float, 7> scales { .2f, .1f, .05f, .1f, .075f, .15f, .175f };

    const std::array <glm::vec2, 7> translationFloorFactor {
        glm::vec2(0.f, 0.f), glm::vec2(-0.6f, -0.4f), glm::vec2(0.6f, -0.6f),
        glm::vec2(0.2f, 0.6f), glm::vec2(-0.2f, -0.6f), glm::vec2(0.6f, 0.6f),
        glm::vec2(-0.6f, 0.6f)
    };

    for (size_t i = 0; i < block.faceInfo().size(); i++) {

        const bool isSharp = block.faceInfo().at(i);
        if (isSharp) {
            constexpr float sizeBlock = 1.f;
            constexpr float offset = sizeBlock / 2.f;

            const JBTypes::Dir currentDir = JBTypesMethods::integerAsDirection(
                static_cast <unsigned int>(i)
            );
            const JBTypes::vec3f vecDir = JBTypesMethods::directionAsVector(currentDir);
            const glm::mat4 translationOffset = glm::translate( glm::vec3(-offset, 0, -offset));
            const glm::mat4 rotationLocal = Utility::rotationUpToDir(currentDir);

            for (size_t j = 0; j < scales.size(); j++) {
                const glm::mat4 scaleLocal = glm::scale(glm::vec3(scales.at(j), 0.5f, scales.at(j)));

                const glm::mat4 translationLocal = glm::translate(glm::vec3(
                    posWorld.x + offset + vecDir.x * offset,
                    posWorld.y + offset + vecDir.y * offset,
                    posWorld.z + offset + vecDir.z * offset
                ));

                const glm::mat4 translationFloor = glm::translate( glm::vec3(
                    offset * translationFloorFactor.at(j).x,
                    0.f,
                    offset * translationFloorFactor.at(j).y
                ));

                const glm::mat4 modelTransf = translationLocal * rotationLocal * translationFloor *
                    scaleLocal * translationOffset;
                const glm::mat4 normalsTransf = rotationLocal;

                geometricShapes.push_back(std::make_shared <Pyramid>(modelTransf, normalsTransf));
            }
        }
    }
    return geometricShapes;
}

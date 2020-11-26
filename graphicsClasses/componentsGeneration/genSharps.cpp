/*
 * File:   genSharps.cpp
 * Author: Morgenthaler S
 *
 * Created on 22 aout 2020, 18h15
 */

#include "scene/mesh/MeshGenerator.h"

vecMeshComponent_sptr MeshGenerator::genSharps(
                                        const BlockState& BlockState,
                                        const Map::BlockTypes& type,
                                        const glm::vec3& posWorld) {

    vecMeshComponent_sptr components;
    if (type == Map::BlockTypes::Sharp) {

        if (commonShapes.find("pyramidSharp") == commonShapes.end()) {
            commonShapes["pyramidSharp"] = std::make_shared<Pyramid> ();
        }

        const std::array<float,7> scales {.2f,.1f,.05f,.1f,.075f,.15f,.175f};

        const std::array<glm::vec2,7> translationFloorFactor
        {
            glm::vec2(0.f,0.f),glm::vec2(-0.6f,-0.4f), glm::vec2(0.6f,-0.6f),
            glm::vec2(0.2f,0.6f), glm::vec2(-0.2f,-0.6f), glm::vec2(0.6f,0.6f),
            glm::vec2(-0.6f,0.6f)
        };

        for(size_t i = 0; i < BlockState.block().faceInfo().size(); i++) {

            const bool isSharp = BlockState.block().faceInfo().at(i);
            if (isSharp) {

                constexpr float sizeBlock = 1.f;
                constexpr float offset = sizeBlock/2.f;

                const JBTypes::Dir currentDir =
                        JBTypesMethods::integerAsDirection(
                            static_cast<unsigned int>(i));
                const JBTypes::vec3f vecDir =
                        JBTypesMethods:: directionAsVector(currentDir);

                const glm::mat4 translationOffset = glm::translate(
                        glm::vec3( -offset, 0 , -offset ));

                const glm::mat4 rotationLocal =
                        Utility::rotationUpToDir(currentDir);


                for (size_t j = 0 ; j < scales.size() ; j++) {
                    const glm::mat4 scaleLocal =
                            glm::scale(glm::vec3(scales.at(j),0.5f,
                            scales.at(j)));

                    const glm::mat4 translationLocal =
                        glm::translate(glm::vec3(
                            posWorld.x + offset + vecDir.x * offset,
                            posWorld.y + offset + vecDir.y * offset,
                            posWorld.z + offset + vecDir.z * offset));

                    const glm::mat4 translationFloor = glm::translate(
                        glm::vec3(
                            offset * translationFloorFactor.at(j).x,
                            0.f,
                            offset * translationFloorFactor.at(j).y));

                    const glm::mat4 modelTransf = translationLocal *
                            rotationLocal * translationFloor * scaleLocal *
                            translationOffset;
                    const glm::mat4 normalsTransf = rotationLocal;

                    MeshComponent_sptr component =
                            std::make_shared<MeshComponent>(
                             std::make_shared<Pyramid>
                    (*commonShapes.at("pyramidSharp"),modelTransf,normalsTransf),
                            nullptr);
                    components.push_back(std::move(component));
                }
            }
        }
    }
    return components;
}

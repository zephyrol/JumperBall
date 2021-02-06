/*
 * File: genJumpers.cpp
 * Author: Morgenthaler S
 *
 * Created on 22 aout 2020, 18h15
 */

#include "scene/mesh/MeshGenerator.h"


/*vecMeshComponent_sptr MeshGenerator::genJumpers (const BlockState& BlockState,
                                                 const Map::BlockTypes& type,
                                                 const glm::vec3& posWorld) {
    vecMeshComponent_sptr components;
    if (type == Map::BlockTypes::Jump) {

        if (commonShapes.find("jumperCylinder") == commonShapes.end()) {
            commonShapes["jumperCylinder"] = std::make_shared <Cylinder>(
                glm::vec3(1.f, 1.f, 0.f),
                glm::vec3(1.f, 150.f / 255.f, 0.f));
        }

        for (size_t i = 0; i < BlockState.block().faceInfo().size(); i++) {
            const bool jumperPresent = BlockState.block().faceInfo().at(i);

            if (jumperPresent) {
                constexpr float sizeBlock = 1.f;
                constexpr float offset = sizeBlock / 2.f;

                const JBTypes::Dir currentDir =
                    JBTypesMethods::integerAsDirection(
                        static_cast <unsigned int>(i));

                const JBTypes::vec3f vecDir =
                    JBTypesMethods::directionAsVector(currentDir);

                const glm::mat4 rotationLocal =
                    Utility::rotationUpToDir(currentDir);

                const glm::mat4 scaleLocal =
                    glm::scale(glm::vec3(0.7f, 0.05f, 0.7f));

                const glm::mat4 translationLocal =
                    glm::translate(glm::vec3(
                                       posWorld.x + offset + vecDir.x * offset,
                                       posWorld.y + offset + vecDir.y * offset,
                                       posWorld.z + offset + vecDir.z * offset
                                       ));

                const glm::mat4 modelTranf = translationLocal *
                                             rotationLocal * scaleLocal;
                const glm::mat4 normalsTrans = rotationLocal;

                MeshComponent_sptr component = std::make_shared <MeshComponent>(
                    std::make_shared <Cylinder>
                        (*commonShapes.at("jumperCylinder"),
                        modelTranf, normalsTrans),
                    nullptr
                    );
                components.push_back(std::move(component));
            }
        }
    }
    return components;

}
*/
//
// Created by S.Morgenthaler on 30/01/2022.
//

#include "SceneUniformBuffer.h"

// SceneUniformBuffer::SceneUniformBuffer(
//     //const vecCstShaderProgram_sptr &sceneShaderPrograms
// ) :
//     _uniformBuffer(UniformBuffer::createInstance(
//         "Scene",
//         sceneShaderPrograms,
//         {
//             "VP",
//             "VPStar",
//             "VPStar2",
//             "cameraPosition",
//             "lightDirection",
//             "light2Direction",
//             "flashColor",
//             "teleportationCoeff"
//         }
//     ))
// {
// }
//
// void SceneUniformBuffer::update(
//     const glm::mat4 &VP,
//     const glm::mat4 &VPStar,
//     const glm::mat4 &VPStar2,
//     const glm::vec3 &cameraPosition,
//     const glm::vec3 &lightDirection,
//     const glm::vec3 &light2Direction,
//     const glm::vec3 &flashColor,
//     const glm::vec1 &teleportationCoeff
// ) {
//
//     _uniformBuffer->updateField(0, VP);
//     _uniformBuffer->updateField(1, VPStar);
//     _uniformBuffer->updateField(2, VPStar2);
//     _uniformBuffer->updateField(3, cameraPosition);
//     _uniformBuffer->updateField(4, lightDirection);
//     _uniformBuffer->updateField(5, light2Direction);
//     _uniformBuffer->updateField(6, flashColor);
//     _uniformBuffer->updateField(7, teleportationCoeff);
//
//     _uniformBuffer->updateBufferOnGPU();
// }



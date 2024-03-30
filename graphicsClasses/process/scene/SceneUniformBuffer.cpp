//
// Created by S.Morgenthaler on 30/01/2022.
//

#include "SceneUniformBuffer.h"

SceneUniformBuffer::SceneUniformBuffer(
    const vecCstShaderProgram_sptr &sceneShaderPrograms
) :
    _uniformBufferBase(UniformBufferBase::createInstance(
        "Scene",
        sceneShaderPrograms,
        {
            "VP",
            "VPStar",
            "VPStar2",
            "cameraPosition",
            "lightDirection",
            "light2Direction",
            "flashColor",
            "teleportationCoeff"
        }
    ))
{
}

void SceneUniformBuffer::update(
    const glm::mat4 &VP,
    const glm::mat4 &VPStar,
    const glm::mat4 &VPStar2,
    const glm::vec3 &cameraPosition,
    const glm::vec3 &lightDirection,
    const glm::vec3 &light2Direction,
    const glm::vec3 &flashColor,
    const glm::vec1 &teleportationCoeff
) {

    _uniformBufferBase.updateField(0, VP);
    _uniformBufferBase.updateField(1, VPStar);
    _uniformBufferBase.updateField(2, VPStar2);
    _uniformBufferBase.updateField(3, cameraPosition);
    _uniformBufferBase.updateField(4, lightDirection);
    _uniformBufferBase.updateField(5, light2Direction);
    _uniformBufferBase.updateField(6, flashColor);
    _uniformBufferBase.updateField(7, teleportationCoeff);

    _uniformBufferBase.updateBufferOnGPU();
}



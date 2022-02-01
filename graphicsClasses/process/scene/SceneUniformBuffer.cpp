//
// Created by Sébastien Morgenthaler on 30/01/2022.
//

#include "SceneUniformBuffer.h"

SceneUniformBuffer::SceneUniformBuffer(const vecCstShaderProgram_sptr &sceneShaderPrograms):
    UniformBuffer("Scene", sceneShaderPrograms,sizeSceneUniformBuffer),
_VP(std::make_shared<glm::mat4>()),
_VPStar(std::make_shared<glm::mat4>()),
_cameraPosition(std::make_shared<glm::vec3>()),
_lightDirection(std::make_shared<glm::vec3>()),
_flashColor(std::make_shared<glm::vec3>()),
_teleportationCoeff(std::make_shared<glm::vec1>()),
_uniformBufferContent(sizeSceneUniformBuffer),
_mat4DataLocations(createMat4DataLocation()),
_vec3DataLocations(createVec3DataLocation()),
_vec1DataLocations(createVec1DataLocation())
{
}

void SceneUniformBuffer::update(
    const glm::mat4 &VP,
    const glm::mat4 &VPStar,
    const glm::vec3 &cameraPosition,
    const glm::vec3 &lightDirection,
    const glm::vec3 &flashColor,
    const glm::vec1 &teleportationCoeff
) {

    updateUniformBufferContent(_VP, VP, _mat4DataLocations);
    updateUniformBufferContent(_VPStar, VPStar, _mat4DataLocations);

    updateUniformBufferContent(_cameraPosition, cameraPosition, _vec3DataLocations);
    updateUniformBufferContent(_lightDirection, lightDirection, _vec3DataLocations);
    updateUniformBufferContent(_flashColor, flashColor, _vec3DataLocations);

    updateUniformBufferContent(_teleportationCoeff, teleportationCoeff, _vec1DataLocations);

    bindBuffer();
    fillBufferData(_uniformBufferContent);
    unbindBuffer();
}

SceneUniformBuffer::DataLocation<glm::mat4> SceneUniformBuffer::createMat4DataLocation() {
    return  {
        { _VP, { _uniformBufferContent.data() + VPOffset, &(*_VP)[0][0] }},
        { _VPStar, {_uniformBufferContent.data() + VPStarOffset, &(*_VPStar)[0][0]}}
    };

}

SceneUniformBuffer::DataLocation<glm::vec3> SceneUniformBuffer::createVec3DataLocation() {
    return {
        { _cameraPosition, { _uniformBufferContent.data() + cameraPositionOffset, &(*_cameraPosition)[0] }},
        { _lightDirection, { _uniformBufferContent.data() + lightDirectionOffset, &(*_lightDirection)[0] }},
        { _flashColor, {_uniformBufferContent.data() + flashColorOffset, &(*_flashColor)[0]}}
    };
}

SceneUniformBuffer::DataLocation<glm::vec1> SceneUniformBuffer::createVec1DataLocation() {
    return  {
        {
            _teleportationCoeff,
            { _uniformBufferContent.data() + teleportationCoeffOffset, &(*_teleportationCoeff)[0] }
        },
    };
}

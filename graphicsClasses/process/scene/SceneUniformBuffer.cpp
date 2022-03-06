//
// Created by Sébastien Morgenthaler on 30/01/2022.
//

#include "SceneUniformBuffer.h"

SceneUniformBuffer::SceneUniformBuffer(
    const vecCstShaderProgram_sptr &sceneShaderPrograms
    ):
    UniformBuffer(
        "Scene",
        sceneShaderPrograms,
        {
            nameVP,
            nameVPStar,
            nameCameraPosition,
            nameLightDirection,
            nameFlashColor,
            nameTeleportationCoeff
        }
    ),
    _VP(std::make_shared<glm::mat4>()),
    _VPStar(std::make_shared<glm::mat4>()),
    _cameraPosition(std::make_shared<glm::vec3>()),
    _lightDirection(std::make_shared<glm::vec3>()),
    _flashColor(std::make_shared<glm::vec3>()),
    _teleportationCoeff(std::make_shared<glm::vec1>()),
    _uniformBufferContent(_bufferSize),
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

    bindBufferRange();
    fillBufferData(_uniformBufferContent);
}

SceneUniformBuffer::DataLocation<glm::mat4> SceneUniformBuffer::createMat4DataLocation() {
    return  {
        { _VP, { _uniformBufferContent.data() + _fieldOffsets.at(nameVP), &(*_VP)[0][0] }},
        { _VPStar, {_uniformBufferContent.data() + _fieldOffsets.at(nameVPStar), &(*_VPStar)[0][0]}}
    };

}

SceneUniformBuffer::DataLocation<glm::vec3> SceneUniformBuffer::createVec3DataLocation() {
    return {
        {
            _cameraPosition,
            { _uniformBufferContent.data() + _fieldOffsets.at(nameCameraPosition), &(*_cameraPosition)[0] }
        },
        {
            _lightDirection,
            { _uniformBufferContent.data() + _fieldOffsets.at(nameLightDirection), &(*_lightDirection)[0] } },
        {
            _flashColor,
            { _uniformBufferContent.data() + _fieldOffsets.at(nameFlashColor), &(*_flashColor)[0] }
        }
    };
}

SceneUniformBuffer::DataLocation<glm::vec1> SceneUniformBuffer::createVec1DataLocation() {
    return  {
        {
            _teleportationCoeff,
            {
                _uniformBufferContent.data() + _fieldOffsets.at(nameTeleportationCoeff),
                &(*_teleportationCoeff)[0]
            }
        },
    };
}


const std::string SceneUniformBuffer::nameVP = "VP";
const std::string SceneUniformBuffer::nameVPStar = "VPStar";
const std::string SceneUniformBuffer::nameCameraPosition = "cameraPosition";
const std::string SceneUniformBuffer::nameLightDirection = "lightDirection";
const std::string SceneUniformBuffer::nameFlashColor = "flashColor";
const std::string SceneUniformBuffer::nameTeleportationCoeff = "teleportationCoeff";

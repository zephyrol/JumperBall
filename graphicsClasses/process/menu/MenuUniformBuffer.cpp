//
// Created by Sébastien Morgenthaler on 30/01/2022.
//

#include "MenuUniformBuffer.h"

MenuUniformBuffer::MenuUniformBuffer(const vecCstShaderProgram_sptr &menuShaderPrograms):
    UniformBuffer("Menu", getBindingPointMap(menuShaderPrograms),sizeMenuUniformBuffer),
_VP(),
_VPStar(),
_cameraPosition(),
_lightDirection(),
_flashColor(),
_teleportationCoeff()
{
}

void MenuUniformBuffer::update(
    const glm::mat4 &VP,
    const glm::mat4 &VPStar,
    const glm::vec3 &cameraPosition,
    const glm::vec3 &lightDirection,
    const glm::vec3 &flashColor,
    float teleportationCoeff
) {
    _VP = VP;
    _VPStar = VPStar;
    _cameraPosition = cameraPosition;
    _lightDirection = lightDirection;
    _teleportationCoeff = teleportationCoeff;
    _flashColor = flashColor;

    constexpr size_t VPOffset = 0;
    constexpr size_t VPStarOffset = sizeof(glm::mat4);
    constexpr size_t cameraPositionOffset = VPStarOffset + sizeof(glm::mat4);
    constexpr size_t lightDirectionOffset = cameraPositionOffset + sizeof(glm::vec3);
    constexpr size_t teleportationCoeffOffset = lightDirectionOffset + sizeof(glm::vec3);
    constexpr size_t flashColorOffset = teleportationCoeffOffset + sizeof(glm::vec3);

    fillBufferData(VPOffset, _VP);
    fillBufferData(VPStarOffset, _VPStar);
    fillBufferData(cameraPositionOffset, _cameraPosition);
    fillBufferData(lightDirectionOffset, _lightDirection);
    fillBufferData(teleportationCoeffOffset, _teleportationCoeff);
    fillBufferData(flashColorOffset, _flashColor);
}

UniformBuffer::ShaderProgramBindingPoint MenuUniformBuffer::getBindingPointMap(
    const vecCstShaderProgram_sptr &menuShaderPrograms
) {
    UniformBuffer::ShaderProgramBindingPoint shaderProgramBindingPoint;
    for (const auto &sp: menuShaderPrograms) {
        // Every menu shader program has only this uniform buffer, so the binding point is 0
        shaderProgramBindingPoint[sp] = 0;
    }

    return shaderProgramBindingPoint;
}

//
// Created by Sébastien Morgenthaler on 30/01/2022.
//

#include "MenuUniformBuffer.h"

MenuUniformBuffer::MenuUniformBuffer(const vecCstShaderProgram_sptr &menuShaderPrograms):
    UniformBuffer("Menu", getBindingPointMap(menuShaderPrograms),sizeMenuUniformBuffer),
    _positionY()
{
}

void MenuUniformBuffer::update(GLfloat positionY ) {
    _positionY = positionY;
    fillBufferData(0, _positionY);
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

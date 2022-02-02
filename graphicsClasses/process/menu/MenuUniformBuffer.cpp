//
// Created by Sébastien Morgenthaler on 30/01/2022.
//

#include "MenuUniformBuffer.h"

MenuUniformBuffer::MenuUniformBuffer(const vecCstShaderProgram_sptr &menuShaderPrograms):
    UniformBuffer("Menu", menuShaderPrograms,{"positionY"}),
    _positionY()
{
}

void MenuUniformBuffer::update(GLfloat positionY) {
    _positionY.x = positionY;
    bindBufferRange();
    fillBufferData(0, _positionY);
}

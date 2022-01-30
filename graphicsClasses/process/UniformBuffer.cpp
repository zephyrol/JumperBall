//
// Created by Sébastien Morgenthaler on 30/01/2022.
//

#include "UniformBuffer.h"

UniformBuffer::UniformBuffer(
    const std::string &name,
    UniformBuffer::ShaderProgramBindingPoint &&shaderProgramsBindingPoint,
    size_t bufferSize):
    _name(name),
    _bufferSize(static_cast<GLsizeiptr>(bufferSize)),
    _shaderProgramToBindingPoint(std::move(shaderProgramsBindingPoint)),
    _ubo(createUbo()){
}

GLuint UniformBuffer::createUbo() const {

    for (const auto &shaderProgramBindingPoint: _shaderProgramToBindingPoint) {
        const auto& shaderProgram = shaderProgramBindingPoint.first;
        const auto& bindingPoint = shaderProgramBindingPoint.second;

        const auto& sp = shaderProgram->getHandle();
        const auto uniformBlockIndex = glGetUniformBlockIndex(sp, _name.c_str());
        glUniformBlockBinding(sp, uniformBlockIndex, bindingPoint);
    }

    GLuint ubo;
    glGenBuffers(1, &ubo);

    glBindBuffer(GL_UNIFORM_BUFFER, ubo);
    glBufferData(GL_UNIFORM_BUFFER, _bufferSize, nullptr, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindBufferRange(GL_UNIFORM_BUFFER, 0, ubo, 0, _bufferSize);

    return ubo;
}

void UniformBuffer::freeGPUMemory() {
    glDeleteBuffers(1, &_ubo);
}

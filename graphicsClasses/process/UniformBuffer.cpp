//
// Created by Sébastien Morgenthaler on 30/01/2022.
//

#include "UniformBuffer.h"

UniformBuffer::UniformBuffer(
    const std::string &name,
    const vecCstShaderProgram_sptr& shaderPrograms,
    size_t bufferSize):
    _name(name),
    _bufferSize(static_cast<GLsizeiptr>(bufferSize)),
    _shaderPrograms(shaderPrograms),
    _ubo(createUbo()){
}

GLuint UniformBuffer::createUbo() const {

    for (const auto &shaderProgram: _shaderPrograms) {

        const auto& sp = shaderProgram->getHandle();
        const auto uniformBlockIndex = glGetUniformBlockIndex(sp, _name.c_str());

        // We always use binding point 0
        glUniformBlockBinding(sp, uniformBlockIndex, 0);
    }

    GLuint ubo;
    glGenBuffers(1, &ubo);

    glBindBuffer(GL_UNIFORM_BUFFER, ubo);
    glBufferData(GL_UNIFORM_BUFFER, _bufferSize, nullptr, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);


    return ubo;
}

void UniformBuffer::bindBuffer() const {
    //glBindBuffer(GL_UNIFORM_BUFFER, _ubo);
    glBindBufferRange(GL_UNIFORM_BUFFER, 0, _ubo, 0, _bufferSize);
}


void UniformBuffer::fillBufferData(const std::vector<GLubyte> &values) {
    //glBindBuffer(GL_UNIFORM_BUFFER, _ubo);
    //glBufferData(GL_UNIFORM_BUFFER, _bufferSize, values.data(), GL_DYNAMIC_DRAW);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, _bufferSize, values.data());
}

void UniformBuffer::unbindBuffer() {
    //glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void UniformBuffer::freeGPUMemory() {
    glDeleteBuffers(1, &_ubo);
}

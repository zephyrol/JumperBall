//
// Created by Sébastien Morgenthaler on 30/01/2022.
//

#include "UniformBuffer.h"

UniformBuffer::UniformBuffer(
    const std::string &name,
    const vecCstShaderProgram_sptr& shaderPrograms,
    const std::vector<std::string>& fieldNames
):
    _name(name),
    _shaderPrograms(shaderPrograms),
    _bufferSize(getBufferSize()),
    _fieldNames(fieldNames),
    _fieldOffsets(getFieldOffsets()),
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

void UniformBuffer::bindBufferRange() const {
    glBindBufferRange(GL_UNIFORM_BUFFER, 0, _ubo, 0, _bufferSize);
}

void UniformBuffer::fillBufferData(const std::vector<GLubyte> &values) {
    glBufferSubData(GL_UNIFORM_BUFFER, 0, _bufferSize, values.data());
}

GLsizeiptr UniformBuffer::getBufferSize() const {
    const auto& spHandle = (*_shaderPrograms.begin())->getHandle();
    const GLuint blockIndex = glGetUniformBlockIndex(spHandle, _name.c_str());
    GLint bufferSize;
    glGetActiveUniformBlockiv(spHandle, blockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &bufferSize);
    return bufferSize;
}

std::unordered_map<std::string, GLint> UniformBuffer::getFieldOffsets() {

    std::vector <const char*> linearFieldNames;
    for (const auto &fieldName : _fieldNames) {
        linearFieldNames.push_back(fieldName.c_str());
    }
    const auto& spHandle = (*_shaderPrograms.begin())->getHandle();

    std::vector<GLuint> indices(_fieldNames.size());
    glGetUniformIndices(spHandle, _fieldNames.size(), linearFieldNames.data(), indices.data());

    std::vector<GLint> offsets(_fieldNames.size());
    glGetActiveUniformsiv(spHandle, _fieldNames.size(), indices.data(), GL_UNIFORM_OFFSET, offsets.data());

    std::unordered_map<std::string, GLint> fieldOffsets {};
    for (size_t i = 0; i < _fieldNames.size(); ++i) {
        fieldOffsets[_fieldNames.at(i)] = offsets[i];
    }

    return fieldOffsets;
}

void UniformBuffer::freeGPUMemory() {
    glDeleteBuffers(1, &_ubo);
}

//
// Created by S.Morgenthaler on 30/01/2022.
//

#include "UniformBufferBase.h"

#include <utility>

UniformBufferBase::UniformBufferBase(
    GLsizeiptr bufferSize,
    const std::vector<GLint> &fieldOffsets,
    CstGpuBuffer_uptr gpuBuffer
) : _uniformBufferSize(bufferSize),
    _uniformBufferContent(_uniformBufferSize),
    _fieldDataLocations(
        std::accumulate(
            fieldOffsets.begin(),
            fieldOffsets.end(),
            std::vector<GLubyte*>(),
            [this](std::vector<GLubyte*> &current, GLint offset) {
                current.emplace_back(_uniformBufferContent.data() + offset);
                return current;
            }
        )
    ),
    _gpuBuffer(std::move(gpuBuffer)) {
}


UniformBufferBase_uptr UniformBufferBase::createInstance(
    const std::string &name,
    const vecCstShaderProgram_sptr &shaderPrograms,
    const std::vector<std::string> &fieldNames
) {
    const auto nameCStr = name.c_str();
    // 1. Get buffer size
    const auto &spHandle = (*shaderPrograms.begin())->getHandle();
    const GLuint blockIndex = glGetUniformBlockIndex(spHandle, nameCStr);
    GLint bufferSize;
    glGetActiveUniformBlockiv(spHandle, blockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &bufferSize);

    // 2. Get field offsets
    std::vector<const char*> linearFieldNames;
    linearFieldNames.reserve(fieldNames.size());
    for (const auto &fieldName: fieldNames) {
        linearFieldNames.push_back(fieldName.c_str());
    }

    std::vector<GLuint> indices(fieldNames.size());
    glGetUniformIndices(
        spHandle,
        static_cast<GLsizei>(fieldNames.size()),
        linearFieldNames.data(),
        indices.data()
    );

    std::vector<GLint> offsets(fieldNames.size());
    glGetActiveUniformsiv(
        spHandle,
        static_cast<GLsizei>(fieldNames.size()),
        indices.data(),
        GL_UNIFORM_OFFSET,
        offsets.data()
    );

    // 3. Create ubo
    for (const auto &shaderProgram: shaderPrograms) {
        const auto &sp = shaderProgram->getHandle();
        const auto uniformBlockIndex = glGetUniformBlockIndex(sp, nameCStr);

        // We always use binding point 0
        glUniformBlockBinding(sp, uniformBlockIndex, 0);
    }

    auto ubo = CstGpuBuffer_uptr(new GpuBuffer());

    glBindBuffer(GL_UNIFORM_BUFFER, ubo->getId());
    glBufferData(GL_UNIFORM_BUFFER, bufferSize, nullptr, GL_DYNAMIC_DRAW);

    // Binding the ubo to the binding point number 0.
    glBindBufferRange(GL_UNIFORM_BUFFER, 0, ubo->getId(), 0, bufferSize);

    return UniformBufferBase_uptr{new UniformBufferBase(
        bufferSize,
        offsets,
        std::move(ubo)
    )};
}

void UniformBufferBase::updateBufferOnGPU() {
    glBufferSubData(GL_UNIFORM_BUFFER, 0, _uniformBufferSize, _uniformBufferContent.data());
}



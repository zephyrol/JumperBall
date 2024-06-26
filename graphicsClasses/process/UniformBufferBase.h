//
// Created by S.Morgenthaler on 30/01/2022.
//

#ifndef JUMPERBALLAPPLICATION_UNIFORMBUFFERBASE_H
#define JUMPERBALLAPPLICATION_UNIFORMBUFFERBASE_H

#include "ShaderProgram.h"
#include <glm/gtc/type_ptr.hpp>

#include "GpuBuffer.h"


class UniformBufferBase;
using UniformBufferBase_uptr = std::unique_ptr<UniformBufferBase>;

class UniformBufferBase {

public:

    UniformBufferBase(
        GLsizeiptr bufferSize,
        const std::vector<GLint> &fieldOffsets,
        CstGpuBuffer_uptr gpuBuffer
    );

    static UniformBufferBase_uptr createInstance(
        const std::string &name,
        const vecCstShaderProgram_sptr &shaderPrograms,
        const std::vector<std::string> &fieldNames
    );

    template<class T>
    void updateField(size_t fieldNumber, const T &value);

    /**
     * Update UBO data. The uniform buffer needs to be bound before.
     */
    void updateBufferOnGPU();

private:
    /**
     * Uniform buffer size. It corresponds to the _uniformBufferContent.size(), but it's store as
     * attribute to avoid cast to GLsizeiptr at each frame.
     */
    const GLsizeiptr _uniformBufferSize;

    /**
     * Uniform buffer content. It's a list of bytes that is sent to the GPU each frame.
     */
    std::vector<GLubyte> _uniformBufferContent;

    /**
     * For each uniform field in the uniform buffer, this attribute stores the related memory address
     * where the field data starts in the uniform buffer content.
     * (A padding is applied by OpenGL between each field).
     */
    const std::vector<GLubyte *> _fieldDataLocations;

    /**
     * Uniform buffer object.
     */
     const CstGpuBuffer_uptr _gpuBuffer;
};

template<class T>
void UniformBufferBase::updateField(size_t fieldNumber, const T &value) {
    // Write field data to its dedicated memory address.
    // _uniformBufferContent is directly modified.
    std::memcpy(_fieldDataLocations[fieldNumber], &value[0], sizeof(T));
}


#endif //JUMPERBALLAPPLICATION_UNIFORMBUFFERBASE_H

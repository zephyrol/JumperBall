//
// Created by S.Morgenthaler on 30/01/2022.
//

#ifndef JUMPERBALLAPPLICATION_UNIFORMBUFFER_H
#define JUMPERBALLAPPLICATION_UNIFORMBUFFER_H

#include <glm/gtc/type_ptr.hpp>
#include "GpuBuffer.h"
#include "ShaderProgram.h"

class UniformBuffer;
using UniformBuffer_uptr = std::unique_ptr<UniformBuffer>;

class UniformBuffer {
   public:
    UniformBuffer(GLsizeiptr bufferSize, const std::vector<GLint>& fieldOffsets, CstGpuBuffer_uptr gpuBuffer);

    static UniformBuffer_uptr createInstance(GLuint bindingPointNumber, GLsizeiptr bufferSize, const std::vector<GLint>& fieldOffsets);




    template <class T>
    void updateField(size_t fieldNumber, const T& value);

    /**
     * Update UBO data. The uniform buffer needs to be bound before.
     */
    void updateBufferOnGPU();

   private:
    /**
     * Name of the uniform buffer in shaders code.
     */
    const std::string _name;

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
    const std::vector<GLubyte*> _fieldDataLocations;

    /**
     * Uniform buffer object.
     */
    const CstGpuBuffer_uptr _gpuBuffer;
};

template <class T>
void UniformBuffer::updateField(size_t fieldNumber, const T& value) {
    // Write field data to its dedicated memory address.
    // _uniformBufferContent is directly modified.
    std::memcpy(_fieldDataLocations[fieldNumber], &value[0], sizeof(T));
}

#endif  // JUMPERBALLAPPLICATION_UNIFORMBUFFER_H

//
// Created by Sebastien Morgenthaler on 21/08/2023.
//

#include "GpuElementBuffer.h"

GpuElementBuffer::GpuElementBuffer(
    GLuint elementBufferObject
) : GpuGeometryBuffer(elementBufferObject) {
}

GpuElementBuffer_sptr GpuElementBuffer::createInstance(const GeometricShape::IndicesBuffer &indices) {

    // 1. Create buffer object.
    const auto bo = createBufferObject();

    // 2. Bind buffer object.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bo);

    // 3. Create data on gpu
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        static_cast<GLsizeiptr>(indices.size() * sizeof(decltype(indices.front()))),
        indices.data(),
        GL_STATIC_DRAW
    );

    return std::make_shared<GpuElementBuffer>(
        bo
    );
}



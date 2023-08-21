//
// Created by S.Morgenthaler on 09/01/2023.
//

#include "GpuVertexBuffer.h"

GpuVertexBuffer::GpuVertexBuffer(
    GLuint vertexBufferObject
):
    _vertexBufferObject(vertexBufferObject) {
}

void GpuVertexBuffer::freeGPUMemory() {
    glDeleteBuffers(1, &_vertexBufferObject);
}

GpuVertexBuffer_sptr GpuVertexBuffer::createInstance(
    const CstVertexAttributeBase_uptr& vertexAttribute,
    GLuint index
) {
    // 1. Create buffer object.
    GLuint bo;
    glGenBuffers(1, &bo);

    // 2. Bind buffer object.
    glBindBuffer(GL_ARRAY_BUFFER, bo);

    // 3. Create data on gpu
    vertexAttribute->createDataOnGpu();

    // 4. Link and activate this vbo in the vao at the index location.
    vertexAttribute->getVertexAttribPointerFunc()(index);
    glEnableVertexAttribArray(index);

    return std::make_shared<GpuVertexBuffer>(
        bo
    );
}

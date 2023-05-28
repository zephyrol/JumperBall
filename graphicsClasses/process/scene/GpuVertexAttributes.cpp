//
// Created by S.Morgenthaler on 09/01/2023.
//

#include "GpuVertexAttributes.h"

GpuVertexAttributes::GpuVertexAttributes(
    GLuint vertexBufferObject,
    const CstVertexAttributeBase_uptr& vertexAttribute,
    GLuint index
) :
    _vertexBufferObject(vertexBufferObject),
    _index(index),
    _vertexAttribPointerFunc(vertexAttribute->getVertexAttribPointerFunc()) {
}

void GpuVertexAttributes::freeGPUMemory() {
    glDeleteBuffers(1, &_vertexBufferObject);
}

void GpuVertexAttributes::bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferObject);
    _vertexAttribPointerFunc(_index);
    glEnableVertexAttribArray(_index);
}

void GpuVertexAttributes::unbind() const {
    glDisableVertexAttribArray(_index);
}

//
// Created by Sebastien Morgenthaler on 09/01/2023.
//

#include "GpuVertexAttributes.h"

GpuVertexAttributes::GpuVertexAttributes(
    GLuint vertexBufferObject,
    CstVertexAttributeBase_uptr vertexAttribute,
    GLuint index
) :
    _vertexBufferObject(vertexBufferObject),
    _vertexAttribute(std::move(vertexAttribute)),
    _index(index) {
}

void GpuVertexAttributes::freeGPUMemory() {
    glDeleteBuffers(1, &_vertexBufferObject);
}

void GpuVertexAttributes::bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferObject);
    glVertexAttribPointer(
        _index,
        _vertexAttribute->numberOfComponents(),
        _vertexAttribute->getDataType(),
        GL_FALSE,
        0,
        nullptr
    );
    glEnableVertexAttribArray(_index);
}

void GpuVertexAttributes::unbind() const {
    glDisableVertexAttribArray(_index);
}

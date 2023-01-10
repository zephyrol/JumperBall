//
// Created by Sebastien Morgenthaler on 09/01/2023.
//

#include "GpuVertexAttributes.h"

GpuVertexAttributes::GpuVertexAttributes(
    GLuint vertexBufferObject,
    const CstVertexAttributeBase_uptr& vertexAttribute,
    GLuint index
) :
    _vertexBufferObject(vertexBufferObject),
    _numberOfComponents(vertexAttribute->numberOfComponents()),
    _dataType(vertexAttribute->getDataType()),
    _index(index) {
}

void GpuVertexAttributes::freeGPUMemory() {
    glDeleteBuffers(1, &_vertexBufferObject);
}

void GpuVertexAttributes::bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferObject);
    glVertexAttribPointer(
        _index,
        _numberOfComponents,
        _dataType,
        GL_FALSE,
        0,
        nullptr
    );
    glEnableVertexAttribArray(_index);
}

void GpuVertexAttributes::unbind() const {
    glDisableVertexAttribArray(_index);
}

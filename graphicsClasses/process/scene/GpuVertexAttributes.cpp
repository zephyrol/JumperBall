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
    _vertexAttribPointerFunc(vertexAttribute->getVertexAttribPointerFunc()),
    _index(index) {
}

void GpuVertexAttributes::freeGPUMemory() {
    glDeleteBuffers(1, &_vertexBufferObject);
}

void GpuVertexAttributes::bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferObject);
    if(_dataType == GL_FLOAT) {
        glVertexAttribPointer(
            _index,
            _numberOfComponents,
            _dataType,
            GL_FALSE,
            0,
            nullptr
        );
    } else if (_dataType == GL_SHORT){
        glVertexAttribIPointer(
            _index,
            _numberOfComponents,
            _dataType,
            0,
            nullptr
        );
    }
    glEnableVertexAttribArray(_index);
}

void GpuVertexAttributes::unbind() const {
    glDisableVertexAttribArray(_index);
}

//
// Created by S.Morgenthaler on 21/08/2023.
//

#include "GpuGeometryBuffer.h"

GpuGeometryBuffer::GpuGeometryBuffer(
    GLuint bufferObject
) : _bufferObject(bufferObject) {
}

void GpuGeometryBuffer::freeGPUMemory() {
    glDeleteBuffers(1, &_bufferObject);
}

GLuint GpuGeometryBuffer::createBufferObject() {
    GLuint bo;
    glGenBuffers(1, &bo);
    return bo;
}

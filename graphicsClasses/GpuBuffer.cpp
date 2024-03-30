//
// Created by S.Morgenthaler on 30/03/2024.
//

#include "GpuBuffer.h"

GpuBuffer::GpuBuffer(): _bufferId(
    []() {
        GLuint buffer;
        glGenBuffers(1, &buffer);
        return buffer;
    }()
) {
}

GLuint GpuBuffer::getId() const {
    return _bufferId;
}

GpuBuffer::~GpuBuffer() {
    glDeleteBuffers(1, &_bufferId);
}

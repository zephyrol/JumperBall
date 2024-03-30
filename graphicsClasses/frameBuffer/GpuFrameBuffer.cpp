//
// Created by S.Morgenthaler on 30/03/2024.
//

#include "GpuFrameBuffer.h"

GpuFrameBuffer::GpuFrameBuffer(): _frameBufferId(
    []() {
        GLuint fbo;
        glGenFramebuffers(1, &fbo);
        return fbo;
    }()
) {
}

void GpuFrameBuffer::bind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, _frameBufferId);
}

GpuFrameBuffer::~GpuFrameBuffer() {
    glDeleteFramebuffers(1, &_frameBufferId);
}

void GpuFrameBuffer::bindDefaultFrameBuffer(GLint defaultFrameBuffer) {
    glBindFramebuffer(GL_FRAMEBUFFER, defaultFrameBuffer);
}

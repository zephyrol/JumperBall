/*
 * File: FrameBuffer.cpp
 * Author: Morgenthaler S
 *
 * Created on 29 janvier 2020, 21:51
 */

#include "FrameBuffer.h"

FrameBuffer::FrameBuffer(GLuint fboHandle, CstTextureSampler_uptr renderTexture):
_fboHandle(fboHandle),
_renderTexture(std::move(renderTexture)){
}

void FrameBuffer::bindFrameBuffer() const {
    glBindFramebuffer(GL_FRAMEBUFFER, _fboHandle);
}

const CstTextureSampler_uptr& FrameBuffer::getRenderTexture() const {
    return _renderTexture;
}

FrameBuffer::~FrameBuffer() {
    glDeleteFramebuffers(1, &_fboHandle);
}

void FrameBuffer::bindDefaultFrameBuffer(GLint defaultFrameBuffer) {
    glBindFramebuffer(GL_FRAMEBUFFER, defaultFrameBuffer);
}

GLuint FrameBuffer::createFrameBufferObject() {
    GLuint fbo;
    glGenFramebuffers(1, &fbo);
    return fbo;
}

void FrameBuffer::setViewportSize(GLsizei resolutionX, GLsizei resolutionY) {
    glViewport(0, 0, resolutionX, resolutionY);
}

void FrameBuffer::enableDepthTest() {
    glEnable(GL_DEPTH_TEST);
}

void FrameBuffer::disableDepthTest() {
    glDisable(GL_DEPTH_TEST);
}

void FrameBuffer::enableBlending() {
    glEnable(GL_BLEND);
}

void FrameBuffer::disableBlending() {
    glDisable(GL_BLEND);
}

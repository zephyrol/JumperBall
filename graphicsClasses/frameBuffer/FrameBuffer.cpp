/*
 * File: FrameBuffer.cpp
 * Author: Morgenthaler S
 *
 * Created on 29 janvier 2020, 21:51
 */

#include "FrameBuffer.h"

FrameBuffer::FrameBuffer(GLuint fboHandle, GLuint renderTexture):
_fboHandle(fboHandle),
_renderTexture(renderTexture){
}

void FrameBuffer::bindFrameBuffer() const {
    glBindFramebuffer(GL_FRAMEBUFFER, _fboHandle);
}

GLuint FrameBuffer::getRenderTexture() const {
    return _renderTexture;
}

void FrameBuffer::freeGPUMemory() {
    glDeleteTextures(1, &_renderTexture);
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

GLuint FrameBuffer::createTexture() {
    GLuint texture;
    glGenTextures(1, &texture);
    return texture;
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


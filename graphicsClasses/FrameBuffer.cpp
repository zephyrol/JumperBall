/*
 * File: FrameBuffer.cpp
 * Author: Morgenthaler S
 *
 * Created on 29 janvier 2020, 21:51
 */

#include "FrameBuffer.h"

FrameBuffer::FrameBuffer(
    GLsizei resolutionX,
    GLsizei resolutionY,
    FrameBuffer::Content content,
    bool hasDepthBuffer,
    const glm::vec3& clearColor
):
    _fboHandle(createFrameBufferObject()),
    _renderTexture(createTexture()),
    _content(content),
    _depthBuffer(hasDepthBuffer ? createDepthBuffer() : nullptr),
    _clearColor(clearColor)
{

    glBindFramebuffer(GL_FRAMEBUFFER, _fboHandle);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _renderTexture);

    const auto getDataFormat =
        [](const FrameBuffer::Content Content) -> GLenum {
        if (Content == FrameBuffer::Content::SDR) {
            return GL_RGB8;
        }
        if (Content == FrameBuffer::Content::HDR) {
            return GL_RGB16F;
        }
        // Depth Map
        return GL_R16F;
    };

    constexpr GLsizei levelTexture = 1;
    const GLenum dataFormat = getDataFormat(_content);

    glTexStorage2D(GL_TEXTURE_2D, levelTexture, dataFormat, resolutionX, resolutionY);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    constexpr GLsizei mipmapLevel = 0;
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _renderTexture, mipmapLevel);

    if (hasDepthBuffer) {
        glBindRenderbuffer(GL_RENDERBUFFER, *_depthBuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, resolutionX, resolutionY);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, *_depthBuffer);
    }
    const GLenum drawBuffer = GL_COLOR_ATTACHMENT0;
    glDrawBuffers(1, &drawBuffer);
}

void FrameBuffer::bindFrameBuffer() const {
    glBindFramebuffer(GL_FRAMEBUFFER, _fboHandle);
}

GLuint FrameBuffer::getRenderTexture() const {
    return _renderTexture;
}

void FrameBuffer::freeGPUMemory() {
    glDeleteTextures(1, &_renderTexture);
    if(_depthBuffer) {
        glDeleteRenderbuffers(1, _depthBuffer.get());
    }
    glDeleteFramebuffers(1, &_fboHandle);
}

void FrameBuffer::bindDefaultFrameBuffer(GLint defaultFrameBuffer) {
    glBindFramebuffer(GL_FRAMEBUFFER, defaultFrameBuffer);
}

void FrameBuffer::cleanDefaultFrameBuffer() {
    cleanCurrentFrameBuffer(false, FrameBuffer::backgroundColor);
}

void FrameBuffer::clean() {
    cleanCurrentFrameBuffer(hasDepthBuffer(), _clearColor);
}

void FrameBuffer::cleanCurrentFrameBuffer (bool hasDepthBuffer, const glm::vec3& clearColor) {
    glClearColor(clearColor.r, clearColor.g, clearColor.z, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    if (hasDepthBuffer) {
        glClear(GL_DEPTH_BUFFER_BIT);
    }
}

GLuint FrameBuffer::createFrameBufferObject() const {
    GLuint fbo;
    glGenFramebuffers(1, &fbo);
    return fbo;
}

GLuint FrameBuffer::createTexture() {
    GLuint texture;
    glGenTextures(1, &texture);
    return texture;
}

std::unique_ptr <GLuint> FrameBuffer::createDepthBuffer() const {
    GLuint depthBuffer;
    glGenRenderbuffers(1, &depthBuffer);
    return std::unique_ptr <GLuint>(new GLuint(depthBuffer));
}

bool FrameBuffer::hasDepthBuffer() const {
    return _depthBuffer ? true : false;
}

GLuint FrameBuffer::getHandle() const {
    return _fboHandle;
}

const glm::vec3 FrameBuffer::backgroundColor { 0.f, 0.f, .1f };

void FrameBuffer::setViewportSize(GLsizei resolutionX, GLsizei resolutionY) {
    glViewport(0, 0, resolutionX, resolutionY);
}

void FrameBuffer::enableDepthTest() {
    glEnable(GL_DEPTH_TEST);
}

void FrameBuffer::disableDepthTest() {
    glDisable(GL_DEPTH_TEST);
}

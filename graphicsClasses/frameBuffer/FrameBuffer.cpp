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

// FrameBuffer::FrameBuffer(
//     GLsizei resolutionX,
//     GLsizei resolutionY,
//     FrameBuffer::Content content,
//     bool hasDepthBuffer,
//     const glm::vec3 &clearColor
// ) :
//     _fboHandle(createFrameBufferObject()),
//     _renderTexture(createTexture()),
//     _depthBuffer(hasDepthBuffer ? createDepthBuffer() : nullptr),
//     _clearColor(clearColor) {
//
//     glBindFramebuffer(GL_FRAMEBUFFER, _fboHandle);
//     glActiveTexture(GL_TEXTURE0);
//     glBindTexture(GL_TEXTURE_2D, _renderTexture);
//
//     const auto getInternalFormat =
//         [](const FrameBuffer::Content Content) -> GLenum {
//             if (Content == FrameBuffer::Content::SDR) {
//                 return GL_RGB8;
//             }
//             if (Content == FrameBuffer::Content::HDR) {
//                 return GL_RGB16F;
//             }
//             // Depth Map
//             return GL_DEPTH_COMPONENT16;
//         };
//
//     const auto getTextureAttachment = [](const FrameBuffer::Content Content) -> GLenum {
//         if(Content == FrameBuffer::Content::Depth) {
//             return GL_DEPTH_ATTACHMENT;
//         }
//         return GL_COLOR_ATTACHMENT0;
//         };
//
//     constexpr GLsizei levelTexture = 1;
//     glTexStorage2D(GL_TEXTURE_2D, levelTexture, getInternalFormat(content), resolutionX, resolutionY);
//
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//
//     constexpr GLsizei mipmapLevel = 0;
//     glFramebufferTexture2D(GL_FRAMEBUFFER, getTextureAttachment(content), GL_TEXTURE_2D, _renderTexture, mipmapLevel);
//
//     if (hasDepthBuffer) {
//         glBindRenderbuffer(GL_RENDERBUFFER, *_depthBuffer);
//         glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, resolutionX, resolutionY);
//         glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, *_depthBuffer);
//     }
//
//     const auto getDrawBuffer= [](const FrameBuffer::Content Content) -> GLenum {
//         if(Content == FrameBuffer::Content::Depth) {
//             return GL_NONE;
//         }
//         return GL_COLOR_ATTACHMENT0;
//     };
//
//     const GLenum drawBuffer = getDrawBuffer(content);
//     glDrawBuffers(1, &drawBuffer);
// }

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


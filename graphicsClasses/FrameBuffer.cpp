/*
 * File: FrameBuffer.cpp
 * Author: Morgenthaler S
 *
 * Created on 29 janvier 2020, 21:51
 */

#include "FrameBuffer.h"

FrameBuffer::FrameBuffer(FrameBuffer::TextureCaterory category,
                         bool usedAutoClean,
                         GLsizei resolutionX,
                         GLsizei resolutionY,
                         bool hasDepthBuffer,
                         const glm::vec3& clearColor):
    _fboHandle(createFrameBufferObject()),
    _renderTexture(createRenderTexture()),
    _textureCategory(category),
    _depthBuffer(hasDepthBuffer ? createDepthBuffer() : nullptr),
    _usedAutoClean(usedAutoClean),
    _clearColor(clearColor),
    _resolutionX(resolutionX),
    _resolutionY(resolutionY) {

    glBindFramebuffer(GL_FRAMEBUFFER, _fboHandle);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _renderTexture);

    const auto getDataFormat = [] (
        const FrameBuffer::TextureCaterory textureCategory)->GLenum {
                                   if (textureCategory == FrameBuffer::TextureCaterory::SDR) {
                                       return GL_RGB8;
                                   } else if (textureCategory == FrameBuffer::TextureCaterory::HDR) {
                                       return GL_RGB16F;
                                   } else { // Depth Map
                                       return GL_R16F;
                                   }
                               };

    constexpr GLsizei levelTexture = 1;
    const GLenum dataFormat = getDataFormat(_textureCategory);

    glTexStorage2D(GL_TEXTURE_2D, levelTexture, dataFormat, resolutionX, resolutionY);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    constexpr GLsizei mipmapLevel = 0;
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _renderTexture, mipmapLevel);

    if (hasDepthBuffer) {
        glBindRenderbuffer(GL_RENDERBUFFER, *_depthBuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, resolutionX, resolutionY);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, *_depthBuffer);
    }
    const GLenum drawBuffer = GL_COLOR_ATTACHMENT0;
    glDrawBuffers(1, &drawBuffer);
    // glBindFramebuffer(GL_FRAMEBUFFER, 0);
    bindDefaultFrameBuffer();
}

void FrameBuffer::bindFrameBuffer() const {
    glBindFramebuffer(GL_FRAMEBUFFER, _fboHandle);
    glViewport(0, 0, _resolutionX, _resolutionY);
    if (_depthBuffer) {
        glEnable(GL_DEPTH_TEST);
    } else {
        glDisable(GL_DEPTH_TEST);
    }
    if (_usedAutoClean) {
        cleanCurrentFrameBuffer(hasDepthBuffer(), _clearColor);
    }
}

GLuint FrameBuffer::getRenderTexture() const {
    return _renderTexture;
}

void FrameBuffer::bindDefaultFrameBuffer() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, Utility::windowResolutionX, Utility::windowResolutionY);
    glDisable(GL_DEPTH_TEST);
}

void FrameBuffer::cleanDefaultFrameBuffer() {
    cleanCurrentFrameBuffer(false, FrameBuffer::backgroundColor);
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

GLuint FrameBuffer::createRenderTexture() const {
    GLuint renderTexture;
    glGenTextures(1, &renderTexture);
    return renderTexture;
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

std::pair <float, float> FrameBuffer::computeLogAverageLuminanceAndMax() const {

    constexpr float epsilon = 0.001f;
    constexpr unsigned int levelOfDetail = 0; // 0 is the base image level
    constexpr unsigned int numberOfComponents = 3; // RGB
    const size_t numberOfPixels = Utility::windowResolutionX * Utility::windowResolutionY;
    std::vector <GLfloat> textureData(numberOfPixels*numberOfComponents);

    // bindRenderTexture(); TODO: update with new architecture if you use it
    glGetTexImage(GL_TEXTURE_2D, levelOfDetail,
                  GL_RGB, GL_FLOAT, textureData.data());

    float sumLogLuminance = 0.f;
    float maxLuminance = 0.f;
    for (size_t i = 0; i < numberOfPixels; ++i) {
        const float luminance = Utility::getLuminance(epsilon + glm::vec3(textureData.at(i),
                                                                          textureData.at(i + 1),
                                                                          textureData.at(i + 2)
                                                                          ));
        sumLogLuminance += logf(luminance);
        if (luminance > maxLuminance) maxLuminance = luminance;
    }

    return std::pair <float, float>(exp(sumLogLuminance / numberOfPixels), maxLuminance);
}


const glm::vec3 FrameBuffer::backgroundColor { 0.f, 0.f, .1f };

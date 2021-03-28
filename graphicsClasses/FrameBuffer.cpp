/*
 * File: FrameBuffer.cpp
 * Author: Morgenthaler S
 *
 * Created on 29 janvier 2020, 21:51
 */

#include "FrameBuffer.h"

FrameBuffer::FrameBuffer(FrameBuffer::TextureCaterory category,
                         GLsizei resolutionX,
                         GLsizei resolutionY,
                         bool hasDepthBuffer):
    _fboHandle(),
    _renderTexture(),
    _textureCategory(category),
    _hasDepthBuffer(hasDepthBuffer),
    _depthBuffer(),
    _resolutionX(resolutionX),
    _resolutionY(resolutionY) {
    constexpr GLsizei levelTexture = 1;
    constexpr GLsizei mipmapLevel = 0;

    glGenFramebuffers(1, &_fboHandle);
    glGenTextures(1, &_renderTexture);

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

    const GLenum dataFormat = getDataFormat(_textureCategory);

    glTexStorage2D(GL_TEXTURE_2D, levelTexture, dataFormat,
                   static_cast <GLsizei>(resolutionX),
                   static_cast <GLsizei>(resolutionY));

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_2D, _renderTexture, mipmapLevel);
    const GLenum drawBuffer = GL_COLOR_ATTACHMENT0;

    if (hasDepthBuffer) {
        glGenRenderbuffers(1, &_depthBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, _depthBuffer);

        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT,
                              resolutionX, resolutionY);

        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                                  GL_RENDERBUFFER, _depthBuffer);
    }
    glDrawBuffers(1, &drawBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::bindFrameBuffer (bool clean) const {
    glBindFramebuffer(GL_FRAMEBUFFER, _fboHandle);

    if (clean) {
        cleanCurrentFrameBuffer(_hasDepthBuffer);
    }
    glViewport(0, 0, _resolutionX, _resolutionY);
}

GLuint FrameBuffer::getRenderTexture() const {
    return _renderTexture;
}

void FrameBuffer::bindDefaultFrameBuffer (bool clean) {

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    if (clean) {
        cleanCurrentFrameBuffer(true);
    }
}

void FrameBuffer::cleanCurrentFrameBuffer (bool hasDepthBuffer) {
    glClear(GL_COLOR_BUFFER_BIT);
    if (hasDepthBuffer) {
        glEnable(GL_DEPTH_TEST);
        glClear(GL_DEPTH_BUFFER_BIT);
    } else {
        glDisable(GL_DEPTH_TEST);
    }
    glViewport(0, 0, Utility::windowResolutionX, Utility::windowResolutionY);
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

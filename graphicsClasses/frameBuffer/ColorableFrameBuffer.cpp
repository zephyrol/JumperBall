//
// Created by S.Morgenthaler on 21/01/2023.
//

#include "ColorableFrameBuffer.h"

ColorableFrameBuffer_uptr ColorableFrameBuffer::createInstance(
    GLsizei resolutionX,
    GLsizei resolutionY,
    bool isHDR,
    bool hasDepthBuffer,
    std::unique_ptr<glm::vec3> clearColor
) {
    const auto fboHandle = createFrameBufferObject();
    const auto renderTexture = createTexture();

    glBindFramebuffer(GL_FRAMEBUFFER, fboHandle);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, renderTexture);

    glTexStorage2D(
        GL_TEXTURE_2D,
        1,
        isHDR ? GL_RGB16F : GL_RGB8,
        resolutionX,
        resolutionY
    );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, isHDR ? GL_NEAREST : GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, isHDR ? GL_NEAREST : GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderTexture, 0);

    std::unique_ptr<GLuint> depthBuffer =
        hasDepthBuffer
        ? [&resolutionX, &resolutionY]() {
            GLuint depthBuffer;
            glGenRenderbuffers(1, &depthBuffer);
            glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, resolutionX, resolutionY);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
            return std::unique_ptr<GLuint>(new GLuint(depthBuffer));
        }()
        : nullptr;

    const GLenum drawBuffer = GL_COLOR_ATTACHMENT0;
    glDrawBuffers(1, &drawBuffer);

    return ColorableFrameBuffer_uptr(new ColorableFrameBuffer(
        fboHandle,
        renderTexture,
        std::move(depthBuffer),
        std::move(clearColor)
    ));
}

ColorableFrameBuffer::ColorableFrameBuffer(
    GLuint fboHandle,
    GLuint renderTexture,
    std::unique_ptr<const GLuint> depthBuffer,
    std::unique_ptr<glm::vec3> clearColor
) : FrameBuffer(fboHandle, renderTexture),
    _depthBuffer(std::move(depthBuffer)),
    _clearColor(std::move(clearColor)) {
}

void ColorableFrameBuffer::freeGPUMemory() {
    if (_depthBuffer) {
        glDeleteRenderbuffers(1, _depthBuffer.get());
    }
    FrameBuffer::freeGPUMemory();
}

void ColorableFrameBuffer::clear() {
    if(_clearColor != nullptr) {
        glClearColor(_clearColor->r, _clearColor->g, _clearColor->z, 0.0f);
        if(_depthBuffer != nullptr) {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        } else {
            glClear(GL_COLOR_BUFFER_BIT);
        }
        return;
    }
    if (_depthBuffer != nullptr) {
        glClear(GL_DEPTH_BUFFER_BIT);
    }
}


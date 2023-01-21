//
// Created by Sebastien Morgenthaler on 21/01/2023.
//

#include "DepthFrameBuffer.h"

DepthFrameBuffer::DepthFrameBuffer(GLuint fboHandle, GLuint renderTexture, const GLuint depthBuffer) :
    FrameBuffer(fboHandle, renderTexture),
    _depthBuffer(depthBuffer) {
}

DepthFrameBuffer_uptr DepthFrameBuffer::createInstance(GLsizei resolutionX, GLsizei resolutionY) {

    const auto fboHandle = createFrameBufferObject();
    const auto renderTexture = createTexture();

    glBindFramebuffer(GL_FRAMEBUFFER, fboHandle);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, renderTexture);

    glTexStorage2D(
        GL_TEXTURE_2D,
        1,
        GL_R16F,
        resolutionX,
        resolutionY
    );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderTexture, 0);

    GLuint depthBuffer;
    glGenRenderbuffers(1, &depthBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, resolutionX, resolutionY);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

    const GLenum drawBuffer = GL_COLOR_ATTACHMENT0;
    glDrawBuffers(1, &drawBuffer);

    return DepthFrameBuffer_uptr (new DepthFrameBuffer(
        fboHandle,
        renderTexture,
        depthBuffer
    ));
}

void DepthFrameBuffer::freeGPUMemory() {
    glDeleteRenderbuffers(1, &_depthBuffer);
    FrameBuffer::freeGPUMemory();
}

void DepthFrameBuffer::clear() {
    glClearColor(1.f, 1.f, 1.f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);
}


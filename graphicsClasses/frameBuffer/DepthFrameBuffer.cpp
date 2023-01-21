//
// Created by Sebastien Morgenthaler on 21/01/2023.
//

#include "DepthFrameBuffer.h"

DepthFrameBuffer::DepthFrameBuffer(GLuint fboHandle, GLuint renderTexture) :
    FrameBuffer(fboHandle, renderTexture) {
}

DepthFrameBuffer_uptr DepthFrameBuffer::createInstance(GLsizei resolutionX, GLsizei resolutionY) {

    const auto fboHandle = createFrameBufferObject();
    const auto depthTexture = createTexture();

    glBindFramebuffer(GL_FRAMEBUFFER, fboHandle);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, depthTexture);

    glTexStorage2D(
        GL_TEXTURE_2D,
        1,
        GL_DEPTH_COMPONENT16,
        resolutionX,
        resolutionY
    );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);

    const GLenum drawBuffer = GL_NONE;
    glDrawBuffers(1, &drawBuffer);

    return DepthFrameBuffer_uptr (new DepthFrameBuffer(
        fboHandle,
        depthTexture
    ));
}

void DepthFrameBuffer::clear() {
    glClear(GL_DEPTH_BUFFER_BIT);
}


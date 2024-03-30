//
// Created by S.Morgenthaler on 21/01/2023.
//

#include "DepthFrameBuffer.h"


DepthFrameBuffer::DepthFrameBuffer(GLuint fboHandle, CstTextureSampler_uptr renderTexture) :
    FrameBuffer(fboHandle, std::move(renderTexture)) {
}

DepthFrameBuffer_uptr DepthFrameBuffer::createInstance(GLsizei resolutionX, GLsizei resolutionY) {

    const auto fboHandle = createFrameBufferObject();
    auto depthTexture = CstTextureSampler_uptr(new TextureSampler());

    glBindFramebuffer(GL_FRAMEBUFFER, fboHandle);
    TextureSampler::setActiveTexture(0);
    depthTexture->bind();


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
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LESS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture->getId(), 0);

    const GLenum drawBuffer = GL_NONE;
    glDrawBuffers(1, &drawBuffer);

    return DepthFrameBuffer_uptr (new DepthFrameBuffer(
        fboHandle,
        std::move(depthTexture)
    ));
}

void DepthFrameBuffer::clear() {
    glClear(GL_DEPTH_BUFFER_BIT);
}


//
// Created by S.Morgenthaler on 21/01/2023.
//

#include "ColorableFrameBuffer.h"

ColorableFrameBuffer_uptr ColorableFrameBuffer::createInstance(
    GLsizei resolutionX,
    GLsizei resolutionY,
    bool useNearestFilter,
    bool hasDepthBuffer,
    std::unique_ptr<glm::vec3> clearColor
) {
    auto renderTexture = CstTextureSampler_uptr(new TextureSampler());
    renderTexture->bind();

    auto gpuFrameBuffer = CstGpuFrameBuffer_uptr(new GpuFrameBuffer());
    gpuFrameBuffer->bind();

    glTexStorage2D(
        GL_TEXTURE_2D,
        1,
        GL_RGBA8,
        resolutionX,
        resolutionY
    );

    const auto filter = useNearestFilter ? GL_NEAREST : GL_LINEAR;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderTexture->getId(), 0);

    CstRenderBuffer_uptr depthBuffer =
        hasDepthBuffer
            ? [&resolutionX, &resolutionY]() {
                auto renderBuffer = CstRenderBuffer_uptr(new RenderBuffer);
                renderBuffer->bind();
                glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, resolutionX, resolutionY);
                glFramebufferRenderbuffer(
                    GL_FRAMEBUFFER,
                    GL_DEPTH_ATTACHMENT,
                    GL_RENDERBUFFER,
                    renderBuffer->getId()
                );
                return renderBuffer;
            }()
            : nullptr;

    const GLenum drawBuffer = GL_COLOR_ATTACHMENT0;
    glDrawBuffers(1, &drawBuffer);

    return ColorableFrameBuffer_uptr(
        new ColorableFrameBuffer(
            std::move(renderTexture),
            std::move(gpuFrameBuffer),
            std::move(depthBuffer),
            std::move(clearColor)
        )
    );
}

ColorableFrameBuffer::ColorableFrameBuffer(
    CstTextureSampler_uptr renderTexture,
    CstGpuFrameBuffer_uptr gpuFrameBuffer,
    CstRenderBuffer_uptr depthBuffer,
    std::unique_ptr<glm::vec3> clearColor
) : FrameBuffer(std::move(renderTexture), std::move(depthBuffer), std::move(gpuFrameBuffer)),
    _clearColor(std::move(clearColor)) {
}

void ColorableFrameBuffer::clear() {
    if (_clearColor != nullptr) {
        glClearColor(_clearColor->r, _clearColor->g, _clearColor->z, 0.0f);

        // If we use a depth buffer
        if (_renderBuffer != nullptr) {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        } else {
            glClear(GL_COLOR_BUFFER_BIT);
        }
        return;
    }
    // If we use a depth buffer
    if (_renderBuffer != nullptr) {
        glClear(GL_DEPTH_BUFFER_BIT);
    }
}
